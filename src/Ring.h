#ifndef RING_H
#define RING_H

#include <Arduino.h>
#include "RingConfig.h"

#define MAX_PIXELS  256             // The total number of pixels supported.

// Define a direction type for the strips and ring. This allows us to use
// a direction like Clock Wise (CW) or Counter Clock Wise (CCW) when referring
// to which way to compute a pixel offset.


class Ring
{
public:

    // Computes the number of pixels on the cage ring.
    uint16_t numPixels();
    uint16_t topCenter();
    uint16_t topQuarter();
    DirectionType ringDirection();

    // Returns the LAST pixel of the cage ring.
    uint16_t lastPixel();
    uint16_t bottomQuarter();
    uint16_t bottomOffset();
    uint16_t halfPixels();
    uint16_t quarterPixels();
    uint16_t incPixel(uint16_t pos);
    uint16_t decPixel(uint16_t pos);

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
