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

uint8_t _patternDefaults[][4] = 
{
    { 0, 0, 0, 255 },
    { 0, 1, 42, 173 },
    { 0, 2, 0, 191 },
    { 1, 0, 157, 0 },
    { 1, 1, 0, 0 },
    { 1, 2, 0, 0 },
    { 1, 3, 0, 0 },
    { 1, 4, 0, 0 },
    { 1, 5, 0, 0 },
    { 1, 6, 0, 0 },
    { 1, 7, 0, 0 },
    { 1, 8, 0, 0 },
    { 2, 0, 0, 0 },
    { 2, 1, 53, 185 },
    { 2, 2, 0, 0 },
    { 2, 3, 55, 210 },
    { 2, 4, 37, 215 },
    { 3, 0, 80, 128 },
    { 3, 1, 0, 113 },
    { 3, 2, 0, 255 },
    { 3, 3, 0, 73 },
    { 3, 4, 0, 48 },
    { 3, 5, 0, 238 },
    { 3, 6, 0, 227 },
    { 4, 0, 0, 34 },
    { 4, 1, 0, 0 },
    { 4, 2, 0, 227 },
    { 4, 3, 0, 227 },
    { 4, 4, 0, 107 },
    { 5, 0, 35, 127 },
    { 5, 1, 38, 107 },
    { 5, 2, 90, 12 },
    { 5, 3, 36, 15 },
    { 5, 4, 36, 0 },
    { 5, 5, 36, 0 },
    { 5, 6, 36, 0 },
    { 5, 7, 36, 0 },
    { 5, 8, 36, 0 },
    { 5, 9, 36, 0 },
    { 6, 0, 38, 9 },
    { 6, 1, 48, 17 },
    { 6, 2, 93, 17 },
    { 6, 3, 0, 17 },
    { 6, 4, 148, 17 }
};

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
        _group = mLastGroup = _patternDefaults[3][0];       // Flag group
        _pattern = mLastPattern = _patternDefaults[3][1];   // American Flag
        mLastData.mSpeed = _patternDefaults[3][2];
        mLastData.mColor = _patternDefaults[3][3];

        EEPROM.write(lastGroupOffset(), mLastGroup);

        for (int group=0; group<=patternGroupType::EMERGENCY_GROUP; group++)
            EEPROM.write(groupOffset(group), mLastPattern);

        for (uint8_t i=0; i<sizeof(_patternDefaults)/sizeof(_patternDefaults[0]); i++)
            writePatternSpeedColor(_patternDefaults[i][0], _patternDefaults[i][1], _patternDefaults[i][2], _patternDefaults[i][3]);

        EEPROM.write(mWriteOffset, MAGIC_NUMBER);
    }
}

void Menu::dumpPatterns()
{
    Serial.begin(9600);

    while (!Serial) ;

    Serial.println("Group,Pattern,Speed,Color");

    for (int group=0; group<=patternGroupType::EMERGENCY_GROUP; group++)
    {
        for (int pattern=0; pattern<_patterns.groupPatternCount((patternGroupType)group); pattern++)
        {
            uint16_t offset = patternOffset(pattern, group);

            Serial.print("{ ");

            Serial.print(group);
            Serial.print(", ");
            Serial.print(pattern);
            Serial.print(", ");
            Serial.print(EEPROM.read(patternSpeedOffset(offset)));
            Serial.print(", ");
            Serial.print(EEPROM.read(patternColorOffset(offset)));

            Serial.println(" },");
        }
    }
}

void Menu::print(const char* prompt, int value)
{
    Serial.print(prompt);
    Serial.print(": ");
    Serial.println(value);
}

void Menu::writeGroupSpeedColor(uint8_t group, uint8_t speed, uint8_t color)
{
    for (int pattern=0; pattern<_patterns.groupPatternCount((patternGroupType)group); pattern++)
        writePatternSpeedColor(group, pattern, speed, color);
}

void Menu::writePatternSpeedColor(uint8_t group, uint8_t pattern, uint8_t speed, uint8_t color)
{
    uint16_t offset = patternOffset(pattern, group);

    EEPROM.write(patternSpeedOffset(offset), speed);
    EEPROM.write(patternColorOffset(offset), color);
}

void Menu::restorePattern(uint8_t group, uint8_t pattern)
{
    uint16_t offset = patternOffset(pattern, group);
    mLastData.mSpeed = EEPROM.read(patternSpeedOffset(offset));
    mLastData.mColor = EEPROM.read(patternColorOffset(offset));

    _speed.saveSensorPosition();
    _color.saveSensorPosition();
}

uint8_t Menu::defaultPattern(uint8_t group)
{
    uint8_t pattern = EEPROM.read(groupOffset(group));

    uint16_t offset = patternOffset(pattern, group);
    mLastData.mSpeed = EEPROM.read(patternSpeedOffset(offset));
    mLastData.mColor = EEPROM.read(patternColorOffset(offset));

    _speed.saveSensorPosition();
    _color.saveSensorPosition();

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

    if (mLastGroup < (uint8_t)patternGroupType::CYCLE_GROUP)
    {
        uint16_t offset = patternOffset(mLastPattern, mLastGroup);

        if (_speed.sensorPositionChanged())
            EEPROM.write(patternSpeedOffset(offset), _speed.value());

        if (_color.sensorPositionChanged())
            EEPROM.write(patternColorOffset(offset), _color.value());
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

    _speed.saveSensorPosition();
    _color.saveSensorPosition();

    _serialDebug.infoInt("|--savedSpeed", _speed.savedSensorPosition());
    _serialDebug.infoInt("|--savedColor", _color.savedSensorPosition());
}

void Menu::updateLastPattern()
{
    _serialDebug.info("|-updateLastPattern");

    mLastPattern = _pattern;

    _serialDebug.infoStr("|--", _patterns.groupName(mLastGroup));
    _serialDebug.infoStr("|--", _patterns.patternName(mLastGroup, mLastPattern));

    readLastPatternData();

    _speed.saveSensorPosition();
    _color.saveSensorPosition();

    _serialDebug.infoInt("|--savedSpeed", _speed.savedSensorPosition());
    _serialDebug.infoInt("|--savedColor", _color.savedSensorPosition());
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
    if (_speed.sensorPositionChanged())
    {
        _serialDebug.infoInt("Speed", _speed.value());
        return _speed.value();
    }

    return mLastData.mSpeed;
}

uint8_t Menu::currentColor()
{
    if (_color.sensorPositionChanged())
    {
        _serialDebug.infoInt("Color", _color.value());
        return _color.value();
    }

    return mLastData.mColor;
}

uint8_t Menu::currentBrightness()
{
    return _bright.read();
}

uint8_t Menu::lastBrightness()
{
    return _bright.lastRead();
}
