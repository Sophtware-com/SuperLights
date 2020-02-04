#ifndef RING_H
#define RING_H

#include <Arduino.h>
#include "RingConfig.h"

#define MAX_PIXELS  240     // The total number of pixels supported.
                            // TODO: Need to make sure no buffer overruns
                            // a the MAX size.

// For patterned LED displays, the more 'divisors' a strand of LEDs has
// the easier it is for Super Lights to map that pattern to the strand.

// Size of a PPC cage ring.
// 312 has 16 divisors: 1 2 3 4 6 8 12 13 24 26 39 52 78 104 156 312

// 4 Meters of LEDs
// 300 18 divisors: 1 2 3 4 5 6 10 12 15 20 25 30 50 60 75 100 150 300

// 3 Meters of LEDs (the standard length we sell)
// 240 has 20 divisors: 1 2 3 4 5 6 8 10 12 15 16 20 24 30 40 48 60 80 120 240

// 2 Meters of LEDs
// 180 has 18 divisors: 1 2 3 4 5 6 9 10 12 15 18 20 30 36 45 60 90 180



// Define a direction type for the strips and ring. This allows us to use
// a direction like Clock Wise (CW) or Counter Clock Wise (CCW) when referring
// to which way to compute a pixel offset.

class Ring
{
private:

    uint16_t mNumPixels;
    uint16_t mLastPixel;
    uint16_t mHalfPixels;
    uint16_t mQuarterPixels;

public:
    void begin()
    {
        mNumPixels = _ringConfig.numPixels();
        mLastPixel = _ringConfig.numPixels() - 1;
        mHalfPixels = _ringConfig.numPixels() / 2;
        mQuarterPixels = _ringConfig.numPixels() / 4;
    }

    // Computes the number of pixels on the cage ring.
    inline uint16_t numPixels()
    {
        return mNumPixels;
    };

    inline uint16_t topCenter()
    {
        return _ringConfig.topCenter();
    }

    inline uint16_t topQuarter()
    {
        return _ringConfig.topQuarter();
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
    uint16_t adjacentPixel(uint16_t pos, DirectionType dir = DirectionType::CW);

    // Can pass in a relative or absolute pixel and the method will Return
    // the pixel on the opposite side of the cage ring.
    uint16_t oppositePixel(uint16_t pos);
};

extern Ring _ring;

#endif // RING_H
