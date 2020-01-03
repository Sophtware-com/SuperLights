#include "RingConfig.h"

#include <EEPROM.h>

#include "SerialDebug.h"
#include "SensorRead.h"
#include "Patterns.h"
#include "Buzzer.h"


RingConfig _ringConfig(12, 6, 3, DirectionType::CCW, 10.0);


void RingConfig::begin(uint16_t writeOffset)
{
    mWriteOffset = writeOffset;
    if (isInitialized())
    {
        int address = mWriteOffset + 1;

        // We store the ring size in multiples of 2.
        mNumPixels = EEPROM.read(address++) << 1;

        mTopCenter = EEPROM.read(address++);
        mTopQuarter = EEPROM.read(address++);
        mDirection = (DirectionType)min(EEPROM.read(address++), 1);
        EEPROM.get(address, mResRatio);

        _serialDebug.info("Ring values restored.");
        _serialDebug.infoInt("|-NumPixels", mNumPixels);
        _serialDebug.infoInt("|-TopCenter", mTopCenter);
        _serialDebug.infoInt("|-TopQuarter", mTopQuarter);
        _serialDebug.infoInt("|-Direction", (int)mDirection);
//        _serialDebug.infoInt("|-SizeOfFloat", sizeof(float));
        _serialDebug.infoFloat("\\-ResRatio", mResRatio);
    }
}

bool RingConfig::isInitialized()
{
    return EEPROM.read(mWriteOffset) == MAGIC_NUMBER;
}

bool RingConfig::bothButtonsOpen()
{
    return (digitalRead(GROUP_BUTTON_PIN) == HIGH) && (digitalRead(PATTERN_BUTTON_PIN) == HIGH);
}

void RingConfig::init()
{
    // Let off the two buttons when this displays.
    _patterns.ledTest(255);
    _buzzer.beep();

    delay(500);

    // Reset to known defaults!
    mNumPixels = 240;
    mTopCenter = 120;
    mTopQuarter = 60;
    mDirection = DirectionType::CCW;
    save();

    // Initialize the numPixels variable...
    while (bothButtonsOpen())
        mNumPixels = _patterns.initializeNumPixels(_speed.read());

    _buzzer.beep();
    delay(500);

    // Initialize the topCenter variable...
    while (bothButtonsOpen())
        mTopCenter = _patterns.initializeTopCenter(min(_speed.read(), mNumPixels), (uint8_t)(mNumPixels/2));

    _buzzer.beep();
    delay(500);

    // Initialize the topQuarter variable...
    while (bothButtonsOpen())
        mTopQuarter = _patterns.initializeTopQuarter(min(_speed.read(), mNumPixels/2));

    mResRatio = 0; // Not used anymore...

    mDirection = (digitalRead(PATTERN_BUTTON_PIN) == HIGH) ? DirectionType::CCW : DirectionType::CW;

    _buzzer.beep();

    save();
}

void RingConfig::save()
{
    int address = mWriteOffset;

    // Now save them back to EEPROM...
    EEPROM.write(address++, MAGIC_NUMBER);
    EEPROM.write(address++, (uint8_t)(mNumPixels>>1));
    EEPROM.write(address++, mTopCenter);
    EEPROM.write(address++, mTopQuarter);
    EEPROM.write(address++, (uint8_t)mDirection);
    EEPROM.put(address, mResRatio);

    _serialDebug.info("Ring values Saved.");
    _serialDebug.infoInt("-NumPixels", mNumPixels);
    _serialDebug.infoInt("-TopCenter", mTopCenter);
    _serialDebug.infoInt("-TopQuarter", mTopQuarter);
    _serialDebug.infoInt("-Direction", (int)mDirection);
    _serialDebug.infoFloat("-ResRatio", mResRatio);
}
