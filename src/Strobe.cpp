#include "Strobe.h"
#include "SerialDebug.h"

Strobe _strobe;


// This maps the ring pixels with Top Dead Center (pixel 0) at the
// top of the cage ring. Since pixel zero starts where the LED strips
// are plugged into, this may not be the most convenient position to
// start from.
uint16_t Strobe::pixel(uint16_t pos, DirectionType dir)
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
