#ifndef RINGCONFIG_H
#define RINGCONFIG_H

#include <Arduino.h>
#include "Global.h"


enum DirectionType
{
    CW = 0,
    CCW = 1
};

class RingConfig
{
private:

    uint16_t mWriteOffset;

    uint16_t mNumPixels;
    uint8_t mTopCenter;
    uint8_t mTopQuarter;
    DirectionType mDirection;

public:
    RingConfig(uint16_t numPixels, uint8_t topCenter, uint8_t topQuarter, DirectionType direction)
    {
        mNumPixels = numPixels;
        mTopCenter = topCenter;
        mTopQuarter = topQuarter;
        mDirection = direction;
    }

    void begin(uint16_t writeOffset = 0);

    bool isInitialized();
    bool bothButtonsOpen();
    void init();
    void save();

    inline uint16_t numPixels() { return mNumPixels; }
    inline uint8_t topCenter() { return mTopCenter; }
    inline uint8_t topQuarter() { return mTopQuarter; }
    inline DirectionType direction() { return mDirection; }
};

extern RingConfig _ringConfig;

#endif // RINGCONFIG_H
