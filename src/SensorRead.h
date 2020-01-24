#ifndef SENSORREAD_H
#define SENSORREAD_H

#include <Arduino.h>
#include "SerialDebug.h"

#define BRIGHT_VAL_PIN     A1
#define SPEED_VAL_PIN      A0
#define COLOR_VAL_PIN      A2

// Cardinal positions of the knobs: full left, middle, full right.
#define POS_UNKNOWN -2
#define POS_LEFT    -1
#define POS_CENTER   0
#define POS_RIGHT    1
#define POS_RANGE   16

class SensorRead
{
private:
    uint16_t *mSamples;
    uint8_t mNumSamples;

    int mPin;
    int mSum;
    int mIndex;
    int mBitShift;
    int mValue;

    bool mFlip;
    uint16_t mFlipper;

    uint16_t mLastRead;
    uint16_t mSavedRead;

public:
    SensorRead(int pin, uint8_t numSamples, int bitShift = 2, bool flip = false)
    {
        mPin = pin;
        mNumSamples = numSamples;

        // Defines the number of bits to shift the ADC read to the right.
        mBitShift = bitShift;

        mFlip = flip;
        mFlipper = (0x3FF >> mBitShift);

        // Allocate the samples array.
        mSamples = new uint16_t[mNumSamples];
    }

    void begin()
    {
        mSum = 0;

        uint16_t sample = Flip(analogRead());

        // Initialize the array with current sensor readings
        // and sum them up for averaging.
        for (mIndex = 0; mIndex<mNumSamples; mIndex++)
        {
            mSamples[mIndex] = sample;
            mSum += mSamples[mIndex];
        }

        // Keep track of last read.
        mValue = mSavedRead = mLastRead = average();
        mIndex = mNumSamples - 1;
    }

    uint16_t Flip(uint16_t value)
    {
        return (mFlip) ? mFlipper - value : value;
    }

    uint16_t average()
    {
        // This returns the average of the samples rounding to the NEAREST
        // number. This helps prevent a reading from vacillating between
        // two numbers. Example: if you had 127.7 and 128.1, then just dividing
        // would always go between 127 and 128. This forces those two to round
        // to 127, and will eliminate I lot of pot jitter.
        return ((mSum < 0) ^ (mNumSamples < 0)) ? ((mSum - mNumSamples/2)/mNumSamples) : ((mSum + mNumSamples/2)/mNumSamples);
    }

    uint16_t read(uint16_t maxValue = 0xFFFF)
    {
        // Store the last read value. We use averaging here to catch those
        // outliers that jump between two values. This will, however, mean
        // that the maximum values you can read decreases by one. So for
        // a BYTE value (8 bits) the range is 0 to 254, not 255.
        mLastRead = (mLastRead + average()) / 2;
        mIndex = (mIndex + 1 >= mNumSamples) ? 0 : mIndex + 1;

        // Instead of always adding the array values, we just
        // remove the current one from the sum and add the new one.
        mSum -= mSamples[mIndex];
        mSamples[mIndex] = Flip(analogRead());
        mSum += mSamples[mIndex];

        mValue = (mLastRead + average()) / 2;

        return min(((uint16_t)mValue), maxValue);
    }

    int knobPosition()
    {
        // The knob is in the middle (12 o'clock, within 16 each side).
        int pos = POS_UNKNOWN;
        uint16_t value = (mFlip) ? Flip(read()) : read();

        pos = POS_CENTER;
        if (value < POS_RANGE)
            pos = POS_RIGHT;
        else if (value > 254-POS_RANGE)
            pos = POS_LEFT;

        return pos;
    }

    uint16_t lastRead()
    {
        // Last read is relevant just after a fresh read.
        return mLastRead;
    }

    uint16_t saveState()
    {
        mSavedRead = read();
        return mSavedRead;
    }

    bool stateChanged()
    {
        if (mSavedRead != read())
        {
            // Once the state changes, keep it that way.
            mSavedRead = 255;
            return true;
        }

        return false;
    }

    uint16_t savedState()
    {
        return mSavedRead;
    }

    uint16_t value()
    {
        return mValue;
    }

    bool hasChanged()
    {
        return (int)mLastRead != mValue;
    }

    uint16_t analogRead()
    {
        // Our read will shift the read value.
        if (mBitShift > 0)
            return (uint16_t)(::analogRead(mPin) >> mBitShift);

        return (uint16_t)(::analogRead(mPin));
    }
};

extern SensorRead _speed;
extern SensorRead _color;
extern SensorRead _bright;

#endif //SENSORREAD_H
