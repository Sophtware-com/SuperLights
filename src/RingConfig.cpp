#include "RingConfig.h"

#include <EEPROM.h>

#include "SerialDebug.h"
#include "SensorRead.h"
#include "Patterns.h"
#include "Buzzer.h"


RingConfig _ringConfig(MAX_PIXELS, MAX_PIXELS/2, MAX_PIXELS/4, DirectionType::CCW);


void RingConfig::begin(uint16_t writeOffset)
{
    mWriteOffset = writeOffset;

    if (isInitialized())
    {
        // Skip the MAGIC number.
        int address = mWriteOffset + 1;

        // We store the ring size in multiples of 2.
        mNumPixels = EEPROM.read(address++) << 1;

        mTopCenter = EEPROM.read(address++);
        mTopQuarter = EEPROM.read(address++);
        mDirection = (DirectionType)min(EEPROM.read(address++), 1);

        _serialDebug.info("Ring values restored.");
        _serialDebug.infoInt("|-NumPixels", mNumPixels);
        _serialDebug.infoInt("|-TopCenter", mTopCenter);
        _serialDebug.infoInt("|-TopQuarter", mTopQuarter);
        _serialDebug.infoInt("|-Direction", (int)mDirection);
    }
    else
    {
        // Standard LED strip.
        mNumPixels = MAX_PIXELS;
        mTopCenter = MAX_PIXELS/2;
        mTopQuarter = MAX_PIXELS/4;
        mDirection = DirectionType::CCW;

        save();
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
    _patterns.displayInit();
    _buzzer.beep();

    delay(500);

    // If we're init'ing the ring, clear out the EEPROM too.
    for (uint16_t i=0; i<EEPROM.length(); i++) 
        EEPROM.write(i, 0);

    // Reset to known defaults!
    mNumPixels = MAX_PIXELS;
    mTopCenter = MAX_PIXELS/2;
    mTopQuarter = MAX_PIXELS/4;
    mDirection = DirectionType::CCW;
    save();

    // Initialize the numPixels variable...
    while (bothButtonsOpen())
        mNumPixels = _patterns.initializeNumPixels(_bright.read());

    _buzzer.beep();
    delay(500);

    // Initialize the topCenter variable...
    while (bothButtonsOpen())
        mTopCenter = _patterns.initializeTopCenter(min(_bright.read(), mNumPixels), (uint8_t)(mNumPixels/2));

    _buzzer.beep();
    delay(500);

    // Initialize the topQuarter variable...
    while (bothButtonsOpen())
        mTopQuarter = _patterns.initializeTopQuarter(min(_bright.read(), mNumPixels/2));

    mDirection = (digitalRead(PATTERN_BUTTON_PIN) == LOW) ? DirectionType::CCW : DirectionType::CW;

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

    _serialDebug.info("Ring values Saved.");
    _serialDebug.infoInt("-NumPixels", mNumPixels);
    _serialDebug.infoInt("-TopCenter", mTopCenter);
    _serialDebug.infoInt("-TopQuarter", mTopQuarter);
    _serialDebug.infoInt("-Direction", (int)mDirection);
}
