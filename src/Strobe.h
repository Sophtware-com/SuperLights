#ifndef STROBE_H
#define STROBE_H

#include <Arduino.h>
#include "RingConfig.h"
#include "global.h"

class Strobe
{
private:

    uint16_t mNumPixels;
    uint16_t mLastPixel;
    uint16_t mHalfPixels;
    uint16_t mQuarterPixels;
    uint16_t mThirdPixels;

public:
    Strobe() 
    {
        mNumPixels = STROBE_LEN;
        mLastPixel = STROBE_LEN - 1;
        mHalfPixels = STROBE_LEN / 2;
        mQuarterPixels = STROBE_LEN / 4;
        mThirdPixels = STROBE_LEN / 3;
    }

    // Computes the number of pixels on the cage ring.
    inline uint16_t numPixels()
    {
        return mNumPixels;
    };

    inline uint16_t topCenter()
    {
        return STROBE_LEN / 2;
    }

    inline uint16_t topQuarter()
    {
        return STROBE_LEN / 4;
    }

    inline DirectionType ringDirection()
    {
        return _ringConfig.direction();
    }

    // Returns the LAST pixel of the cage ring.
    inline uint16_t lastPixel()
    {
        return mLastPixel;
    };

    inline uint16_t bottomQuarter()
    {
        return halfPixels() - topQuarter();
    }

    inline uint16_t bottomOffset()
    {
        return topQuarter() - bottomQuarter();
    }

    inline uint16_t halfPixels()
    {
        return mHalfPixels;
    }

    inline uint16_t quarterPixels()
    {
        return mQuarterPixels;
    }

    inline uint16_t thirdPixels()
    {
        return mThirdPixels;
    }

    inline uint16_t incPixel(uint16_t pos)
    {
        return (pos == mLastPixel) ? 0 : pos + 1;
    }

    inline uint16_t decPixel(uint16_t pos)
    {
        return (pos == 0) ? mLastPixel : pos - 1;
    }

    // This maps the ring pixels with Top Dead Center (pixel 0) at the
    // top of the cage ring. Since pixel zero starts where the LED strips
    // are plugged into, this may not be the most convenient position to
    // start from.
    uint16_t pixel(uint16_t pos, DirectionType dir = DirectionType::CW);

    // Can pass in a relative or absolute pixel and the mehtod will return
    // the pixel to the LEFT (CCW) or RIGHT (CW).
    //uint16_t adjacentPixel(uint16_t pos, DirectionType dir = DirectionType::CW);

    // Can pass in a relative or absolute pixel and the method will Return
    // the pixel on the opposite side of the cage ring.
    //uint16_t oppositePixel(uint16_t pos);
};

extern Strobe _strobe;

#endif // STROBE_H
