#include "Menu.h"
#include "Global.h"
#include "SerialDebug.h"
#include "SensorRead.h"
#include "Patterns.h"

#include <EEPROM.h>

#define DEFAULT_SPEED 80
#define DEFAULT_COLOR 128


Menu _menu;

// These get updated by our button interrupts.
volatile uint8_t _group;
volatile uint8_t _pattern;


// EEPROM MEMORY LAYOUT
//
//   -RING CONFIG-
//   ⌜ 00 - MAGIC_NUMBER
//   | 01 - NumPixels >> 1 (div 2)
//   | 02 - TopCenter
//   | 03 - TopQuarter
//   ⌞ 04 - Direction (CW=0, CCW=1)
//
//   -PATTERN GROUPS-
//   ⌜ 00 - MAGIC_NUMBER
//   ⌞ 01 - LastGroup
//   -PATTERNS-
//   ⌜ 02.00 - LastPattern
//   | 03.01 - P1.Speed
// G | 04.02 - P1.Color
// 0 | ...
//   | 22.19 - P10.SPEED
//   ⌞ 23.20 - P10.Color
//   ⌜ 24.00 - LastPattern
//   | 25.01 - P1.Speed
// G | 26.02 - P1.Color
// 1 | ...
//   | 44.19 - P10.SPEED
//   ⌞ 45.20 - P10.Color
//   ⌜
//     ...
//   ⌞
//   ⌜ 212.00 - LastPattern
//   | 213.01 - P1.Speed
// G | 214.02 - P1.Color
// 7 | ...
//   | 10.19 - P10.SPEED
//   ⌞ 11.20 - P10.Color

uint16_t Menu::lastGroupOffset()
{
    return mWriteOffset + 1;
}

uint16_t Menu::groupOffset(uint8_t group)
{
    return (((MAX_PATTERNS * 2) + 1) * group) + mWriteOffset + 2;
}

uint16_t Menu::patternOffset(uint8_t pattern, uint8_t group)
{
    return groupOffset(group) + (pattern * 2) +  1;
}

uint16_t Menu::patternSpeedOffset(uint16_t patternOffset)
{
    return patternOffset;
}

uint16_t Menu::patternColorOffset(uint16_t patternOffset)
{
    return patternOffset + 1;
}


bool Menu::isInitialized()
{
    return EEPROM.read(mWriteOffset) == MAGIC_NUMBER;
}

void Menu::begin(uint16_t writeOffset)
{
    // Where to start in memory for this data.
    mWriteOffset = writeOffset;

    // Check for the magic number. This lets us know we've initialized.
    if (isInitialized())
    {
        _serialDebug.info("Restored Last Group/Pattern");

        // Read in the last Group used, and the last Pattern in that group.
        _group = mLastGroup = EEPROM.read(lastGroupOffset());
        _pattern = mLastPattern = EEPROM.read(groupOffset(mLastGroup));

        _serialDebug.infoStr("|-", _patterns.groupName(mLastGroup));
        _serialDebug.infoStr("|-", _patterns.patternName(mLastGroup, mLastPattern));

        // Now get the data for the last pattern.
        uint16_t offset = patternOffset(mLastPattern, mLastGroup);
        mLastData.mSpeed = EEPROM.read(patternSpeedOffset(offset));
        mLastData.mColor = EEPROM.read(patternColorOffset(offset));

        _serialDebug.infoInt("|--speed", mLastData.mSpeed);
        _serialDebug.infoInt("|--color", mLastData.mColor);
    }
    else
    {
        _group = mLastGroup = 1;        // Flag group
        _pattern = mLastPattern = 1;    // FlatTop American Flag
        mLastData.mSpeed = DEFAULT_SPEED;
        mLastData.mColor = DEFAULT_COLOR;

        EEPROM.write(lastGroupOffset(), mLastGroup);

        for (int i=0; i<MAX_GROUPS; i++)
        {
            EEPROM.write(groupOffset(i), mLastPattern);
            for (int j=0; j<MAX_PATTERNS; j++)
            {
                uint16_t offset = patternOffset(j, i);
                EEPROM.write(patternSpeedOffset(offset), mLastData.mSpeed);
                EEPROM.write(patternColorOffset(offset), mLastData.mColor);
            }
        }

        EEPROM.write(mWriteOffset, MAGIC_NUMBER);
    }
}

void Menu::restorePattern(uint8_t group, uint8_t pattern)
{
    uint16_t offset = patternOffset(pattern, group);
    mLastData.mSpeed = EEPROM.read(patternSpeedOffset(offset));
    mLastData.mColor = EEPROM.read(patternColorOffset(offset));

    _speed.savedState();
    _color.saveState();
}

uint8_t Menu::defaultPattern(uint8_t group)
{
    uint8_t pattern = EEPROM.read(groupOffset(group));

    uint16_t offset = patternOffset(pattern, group);
    mLastData.mSpeed = EEPROM.read(patternSpeedOffset(offset));
    mLastData.mColor = EEPROM.read(patternColorOffset(offset));

    _speed.savedState();
    _color.saveState();

    return pattern;
}


bool Menu::groupChanged()
{
    return mLastGroup != _group;
}

bool Menu::patternChanged()
{
    return mLastPattern != _pattern;
}


void Menu::writeLastMenu()
{
    writeLastGroup();
    writeLastPattern();
    writeLastPatternData();
}

void Menu::writeLastGroup()
{
    _serialDebug.info("|-writeLastGroup");
    _serialDebug.infoStr("|--", _patterns.groupName(mLastGroup));

    EEPROM.write(lastGroupOffset(), mLastGroup);
}

void Menu::writeLastPattern()
{
    _serialDebug.info("|-writeLastPattern");
    _serialDebug.infoStr("|--", _patterns.patternName(mLastGroup, mLastPattern));

    EEPROM.write(groupOffset(mLastGroup), mLastPattern);
}

void Menu::writeLastPatternData()
{
    _serialDebug.info("|-writeLastPatternData");

    if (_speed.stateChanged() || _color.stateChanged())
    {
        if (mLastGroup != (uint8_t)patternGroupType::CYCLE_GROUP)
        {
            uint16_t offset = patternOffset(mLastPattern, mLastGroup);
            EEPROM.write(patternSpeedOffset(offset), _speed.value());
            EEPROM.write(patternColorOffset(offset), _color.value());

            _serialDebug.infoInt("|--speed", _speed.value());
            _serialDebug.infoInt("|--color", _color.value());
        }
        else
        {
            _serialDebug.info("|--skipped cycleGroup.");
        }
    }
    else
    {
        _serialDebug.info("|--skipped.");
    }
}


void Menu::updateLastGroup(bool displayLastPattern)
{
    _serialDebug.info("|-updateLastGroup");

    mLastGroup = _group;
    mLastPattern = _pattern = (displayLastPattern) ? readLastPattern() : _pattern;

    _serialDebug.infoStr("|--", _patterns.groupName(mLastGroup));
    _serialDebug.infoStr("|--", _patterns.patternName(mLastGroup, mLastPattern));

    readLastPatternData();

    _speed.saveState();
    _color.saveState();

    _serialDebug.infoInt("|--savedSpeed", _speed.savedState());
    _serialDebug.infoInt("|--savedColor", _color.savedState());
}

void Menu::updateLastPattern()
{
    _serialDebug.info("|-updateLastPattern");

    mLastPattern = _pattern;

    _serialDebug.infoStr("|--", _patterns.groupName(mLastGroup));
    _serialDebug.infoStr("|--", _patterns.patternName(mLastGroup, mLastPattern));

    readLastPatternData();

    _speed.saveState();
    _color.saveState();

    _serialDebug.infoInt("|--savedSpeed", _speed.savedState());
    _serialDebug.infoInt("|--savedColor", _color.savedState());
}


uint8_t Menu::readLastGroup()
{
    return EEPROM.read(lastGroupOffset());
}

uint8_t Menu::readLastPattern()
{
    return EEPROM.read(groupOffset(mLastGroup));
}

Pattern Menu::readLastPatternData()
{
    _serialDebug.info("|-readLastPatternData");

    uint16_t offset = patternOffset(mLastPattern, mLastGroup);
    mLastData.mSpeed = EEPROM.read(patternSpeedOffset(offset));
    mLastData.mColor = EEPROM.read(patternColorOffset(offset));

    _serialDebug.infoInt("|--readSpeed", mLastData.mSpeed);
    _serialDebug.infoInt("|--readColor", mLastData.mColor);

    return mLastData;
}


uint8_t Menu::currentSpeed()
{
    if (_speed.stateChanged())
    {
        _serialDebug.infoInt("Speed", _speed.value());
        return _speed.value();
    }

    return mLastData.mSpeed;
}

uint8_t Menu::currentColor()
{
    if (_color.stateChanged())
    {
        _serialDebug.infoInt("Color", _color.value());
        return _color.value();
    }

    return mLastData.mColor;
}

uint8_t Menu::currentBrightness(uint8_t max)
{
    return _bright.read(max);
}

uint8_t Menu::lastBrightness()
{
    return _bright.lastRead();
}
