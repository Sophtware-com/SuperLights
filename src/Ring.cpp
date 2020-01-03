#include "Ring.h"
#include "SerialDebug.h"

Ring _ring;


// Computes the number of pixels on the cage ring.
uint16_t Ring::numPixels()
{
    return _ringConfig.numPixels();
};

uint16_t Ring::topCenter()
{
    return _ringConfig.topCenter();
}

uint16_t Ring::topQuarter()
{
    return _ringConfig.topQuarter();
}

DirectionType Ring::ringDirection()
{
    return _ringConfig.direction();
}

// Returns the LAST pixel of the cage ring.
uint16_t Ring::lastPixel()
{
    return numPixels() - 1;
};

uint16_t Ring::bottomQuarter()
{
    return halfPixels() - topQuarter();
}

uint16_t Ring::bottomOffset()
{
    return topQuarter() - bottomQuarter();
}

uint16_t Ring::halfPixels()
{
    return numPixels() / 2;
}

uint16_t Ring::quarterPixels()
{
    return numPixels() / 4;
}

uint16_t Ring::incPixel(uint16_t pos)
{
    return ((pos+1) >= numPixels()) ? 0 : pos + 1;
}

uint16_t Ring::decPixel(uint16_t pos)
{
    return (pos <= 0) ? lastPixel() : pos -1;
}

// This maps the ring pixels with Top Dead Center (pixel 0) at the
// top of the cage ring. Since pixel zero starts where the LED strips
// are plugged into, this may not be the most convenient position to
// start from.
uint16_t Ring::pixel(uint16_t pos, DirectionType dir)
{
    // The easy way to get a pixel position on the ring when looking in
    // the OPPOSITE direction of the data flow, is to compute what that
    // pixel position would be if the direction was the other way. So on
    // a 200 pixel ring running CCW, if you want position 10 CW, that would
    // be the equivalent to 190 CCW.
    pos = (dir == ringDirection()) ? pos : lastPixel() - pos;

    // Now we can compute the pixel position always in the same direction
    // as the data flow around the ring, which is much easier.
    uint16_t newPos = pos + topCenter();

    // If we pasted the end, just subtract the len of ring to get the pos.
    if (newPos >= numPixels())
        return newPos - numPixels();

    return newPos;
}

// Can pass in a relative or absolute pixel and the mehtod will return
// the pixel to the LEFT (CCW) or RIGHT (CW).
uint16_t Ring::adjacentPixel(uint16_t pos, DirectionType dir)
{
    if (dir == CW)
        return ((pos+1) == numPixels()) ? 0 : pos + 1;
    else
        return (pos == 0) ? numPixels()-1 : pos - 1;
}

// Can pass in a relative or absolute pixel and the method will Return
// the pixel on the opposite side of the cage ring.
uint16_t Ring::oppositePixel(uint16_t pos)
{
    return (pos + (numPixels()/2)) % numPixels();
}
