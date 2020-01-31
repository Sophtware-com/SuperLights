#include "Patterns.h"


Patterns *_patterns;


//
// GROUP & PATTERN INITI
//
const uint8_t Patterns::maxGroups()
{
    return (uint8_t)patternGroupType::NUMBER_OF_GROUPS;
}

const uint8_t Patterns::maxGroupPatterns()
{
    return 10; // Arbitrary limit to patterns in a group.
}

const uint8_t Patterns::groupPatternCount(patternGroupType group)
{
    switch (group) {
        case STROBE_GROUP:
            return 3;
        case FLAG_GROUP:
            return 7;
        case RAINBOW_GROUP:
            return 4;
        case COLOR_GROUP:
            return 7;
        case BOUNCE_GROUP:
             return 5;
        case HOLIDAY_GROUP:
            return 8;
        case EMERGENCY_GROUP:
            return 5;
        case CYCLE_GROUP:
            return 1;
        case CYCLE_ALL_GROUP:
            return 1;
        default:
            return 0;
    }
}

const char* Patterns::groupName(uint8_t group)
{
#if defined(USE_SERIAL)

    static char buffer[20];
    const PROGMEM static char names[][20] =
    {
        "STROBE_GROUP",
        "FLAG_GROUP",
        "RAINBOW_GROUP",
        "COLOR_GROUP",
        "BOUNCE_GROUP",
        "HOLIDAY_GROUP",
        "EMERGENCY_GROUP",
        "CYCLE_GROUP",
        "CYCLE_ALL_GROUP",
    };

    strcpy_P(buffer, (char*)&(names[group]));

    return buffer;

#else

    return "";

#endif
}

const char* Patterns::patternName(uint8_t group, uint8_t pattern)
{
#if defined(USE_SERIAL)

    // This array was causing a stack overflow, so I had to move the data
    // to the program memory space.

    static char buffer[32];
    const PROGMEM static char names[][10][32] =
    {
        { // STROBE_GROUP
            "doubleStrobe",
            "tripleStrobe",
            "aircraftStrobe",
            "3",
            "4",
            "5"
            "6",
            "7",
            "8",
            "9"
        },
        { // FLAG_GROUP
            "americanFlag",
            "spainFlag",
            "mexicanFlag",
            "frenchFlag",
            "canadianFlag",
            "portugalFlag",
            "rebelFlag",
            "7",
            "8",
            "9"
        },
        { // RAINBOW_GROUP
            "rainbowFadeWave",
            "rainbowTheaterWave",
            "rainbowFade",
            "rainbowTheater",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9"
        },
        { // COLOR_GROUP
            "onFire",
            "comet",
            "fireFlies",
            "randomPixels",
            "flickerColor",
            "starBurst",
            "solidColor",
            "7",
            "8",
            "9"
        },
        { // BOUNCE_GROUP
            "rainbowNightRider",
            "rainbowQuadRider",
            "colorNightRider",
            "colorQuadRider",
            "bouncingBalls",
            "5",
            "6",
            "7",
            "8",
            "9"
        },
        { // HOLIDAY_GROUP
            "holidaySparkle",
            "christmasLights",
            "valentineLights",
            "saintPatrickLights",
            "easterLights",
            "independenceLights",
            "halloweenLights",
            "thanksGivingLights",
            "8",
            "9"
        },
        { // EMERGENCY_GROUP
            "chasingPoliceLights",
            "redBlueTripleSegmentFlash",
            "redBlueHalfRingFlash",
            "redBlueHalfRingCrawl",
            "redBlueTripleFlash"
            "5",
            "6",
            "7",
            "8",
            "9"
        },
        { // CYCLE_GROUP
            "cycleFavorites",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9"
        },
        { // CYCLE_ALL_GROUP
            "cycleAll",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9"
        },

    };

    strcpy_P(buffer, (char*)&(names[group][pattern]));

    return buffer;

#else

    return "";

#endif
}


//
// DISPLAY PATTERNS
//
void Patterns::displayPattern()
{
    displayPattern(_group);
}

void Patterns::displayPattern(uint8_t group)
{
    switch ((patternGroupType)group) {
        default:
        case STROBE_GROUP:
            strobeGroup();
            break;
        case FLAG_GROUP:
            flagGroup();
            break;
        case RAINBOW_GROUP:
            rainbowGroup();
            break;
        case COLOR_GROUP:
            colorGroup();
            break;
        case BOUNCE_GROUP:
            bounceGroup();
            break;
        case HOLIDAY_GROUP:
            holidayGroup();
            break;
        case EMERGENCY_GROUP:
            emergencyGroup();
            break;
        case CYCLE_GROUP:
            cycleGroup();
            break;
        case CYCLE_ALL_GROUP:
            cycleAllGroup();
            break;
    }
}

void Patterns::displayPattern(uint8_t group, uint8_t pattern)
{
    switch ((patternGroupType)group) {
        default:
        case STROBE_GROUP:
            strobeGroup(pattern);
            break;
        case FLAG_GROUP:
            flagGroup(pattern);
            break;
        case RAINBOW_GROUP:
            rainbowGroup(pattern);
            break;
        case COLOR_GROUP:
            colorGroup(pattern);
            break;
        case BOUNCE_GROUP:
            bounceGroup(pattern);
            break;
        case HOLIDAY_GROUP:
            holidayGroup(pattern);
            break;
        case EMERGENCY_GROUP:
            emergencyGroup(pattern);
            break;
        case CYCLE_GROUP:
            cycleGroup();
            break;
        case CYCLE_ALL_GROUP:
            cycleAllGroup();
            break;
    }
}


//
//  STROBE GROUP
//
void Patterns::strobeGroup()
{
    strobeGroup(_pattern);
}

void Patterns::strobeGroup(uint8_t pattern)
{
    switch (pattern) {
        case 0:
        default:
            doubleStrobe();
            break;
        case 1:
            tripleStrobe();
            break;
        case 2:
            aircraftStrobe();
            break;
    }
}

void Patterns::doubleStrobe()
{
    if (isFirstFrame())
    {
        mFrame.maxFrames = 10;
        mFrame.currentFrame = 0;
    }

    nextFrame();

    if (mFrame.currentFrame < 3)
    {
        uint8_t width = max(1,(uint8_t)(((float)_menu.currentColor() * ((float)_ring.numPixels()/254.0)) / 2.0));

        setPixelColor(0, white(), width);
        setPixelColor(0, white(), width, CCW);
        show(50);
        clear();
        show(50);
    }
    else
    {
        delay(100);
    }
}

void Patterns::tripleStrobe()
{
    if (isFirstFrame())
    {
        mFrame.maxFrames = 10;
        mFrame.currentFrame = 0;
    }

    nextFrame();

    if (mFrame.currentFrame < 4)
    {
        uint8_t width = max(1,(uint8_t)(((float)_menu.currentColor() * ((float)_ring.numPixels()/254.0)) / 2.0));

        setPixelColor(0, white(), width);
        setPixelColor(0, white(), width, CCW);
        show(50);
        clear();
        show(50);
    }
    else
    {
        delay(100);
    }
}

void Patterns::aircraftStrobe()
{
    if (isFirstFrame())
    {
        mFrame.maxFrames = 10;
        mFrame.currentFrame = 0;
    }

    nextFrame();

    initBrightness();
    initSize();

    setPixelColor(0, red(knobs.brightness), _ring.halfPixels());
    setPixelColor(0, green(knobs.brightness), _ring.halfPixels(), CCW);

    if (mFrame.currentFrame < 3)
    {
        uint8_t topOffset = _ring.bottomOffset();
        uint8_t size = map(knobs.size,1,254,6,24);
    
        setPixelColor(0, white(), size+topOffset);
        setPixelColor(0, white(), size+topOffset, CCW);
        setPixelColor(_ring.halfPixels()-size, white(), size);
        setPixelColor(_ring.halfPixels()-size, white(), size, CCW);
        show(50);
        setPixelColor(0, 0, size+topOffset);
        setPixelColor(0, 0, size+topOffset, CCW);
        setPixelColor(_ring.halfPixels()-size, 0, size);
        setPixelColor(_ring.halfPixels()-size, 0, size, CCW);
        show(50);
    }
    else
    {
        delay(100);
    }
}


//
//  FLAG GROUP
//
void Patterns::flagGroup()
{
    flagGroup(_pattern);
}

void Patterns::flagGroup(uint8_t pattern)
{
    switch (pattern) {
        case 0: // AMERICAN
        default:
            americanFlag(); break;
        case 1: // SPAIN
            spainFlag(); break;
        case 2: // MEXICAN
            mexicanFlag(); break;
        case 3: // FRENCH
            frenchFlag(); break;
        case 4: // CANADIAN
            canadianFlag(); break;
        case 5: // PORTUGAL
            portugalFlag(); break;
        case 6: // REBEL
            rebelFlag(); break;
    }
}

void Patterns::americanFlag()
{
    initBrightness();
    initSpeed();

    if (isFirstFrame())
    {
        // ALL WHITE STRIPES
        setRingColor(white(knobs.brightness));

        // STRIPS RIGHT
        uint8_t stripWidth = _ring.topQuarter() / 7;
        uint8_t extraPixels = _ring.topQuarter() % 7;

        setPixelColor(0, red(knobs.brightness), extraPixels, CW);
        setPixelColor(extraPixels, red(knobs.brightness), _ring.topQuarter(), CW, stripWidth*2, stripWidth);

        // Strips Bottom
        stripWidth = _ring.bottomQuarter() / 6;
        extraPixels = _ring.bottomQuarter() % 6;

        setPixelColor(_ring.topQuarter()+stripWidth, red(knobs.brightness), _ring.bottomQuarter()-stripWidth, CW, stripWidth*2, stripWidth);
        setPixelColor(_ring.topQuarter()+stripWidth, red(knobs.brightness), _ring.bottomQuarter()-stripWidth, CCW, stripWidth*2, stripWidth);
    }

    // Blue Panel
    setPixelColor(0, blue(knobs.brightness), _ring.topQuarter(), CCW);

    // Stars
    uint8_t spacing = _ring.topQuarter() / 6;

    if (twinkle())
        setPixelColor(spacing-1, white(knobs.brightness), spacing*4+2, CCW, spacing, 2);
    else
        setPixelColor(spacing-1+(spacing/2), white(knobs.brightness), spacing*3+2, CCW, spacing, 2);

    show(knobs.speed);
}

void Patterns::spainFlag()
{
    if (isFirstFrame())
    {
        initBrightness();

        uint8_t topOffset = _ring.bottomOffset();

        setPixelColor(0, yellow(knobs.brightness), _ring.halfPixels());
        setPixelColor(0, yellow(knobs.brightness), _ring.halfPixels(), CCW);

        setPixelColor(0, red(knobs.brightness), 12+topOffset);
        setPixelColor(0, red(knobs.brightness), 12+topOffset, CCW);
        setPixelColor(_ring.halfPixels()-12, red(knobs.brightness), 12);
        setPixelColor(_ring.halfPixels()-12, red(knobs.brightness), 12, CCW);

        show(200);
    }
}

void Patterns::mexicanFlag()
{
    if (isFirstFrame())
    {
        initBrightness();

        uint8_t topOffset = _ring.bottomOffset();

        setPixelColor(0, red(knobs.brightness), _ring.halfPixels());
        setPixelColor(0, green(knobs.brightness), _ring.halfPixels(), CCW);

        setPixelColor(0, white(knobs.brightness), 12+topOffset);
        setPixelColor(0, white(knobs.brightness), 12+topOffset, CCW);
        setPixelColor(_ring.halfPixels()-12, white(knobs.brightness), 12);
        setPixelColor(_ring.halfPixels()-12, white(knobs.brightness), 12, CCW);

        show(200);
    }
}

void Patterns::frenchFlag()
{
    if (isFirstFrame())
    {
        initBrightness();

        uint8_t topOffset = _ring.bottomOffset();

        setPixelColor(0, red(knobs.brightness), _ring.halfPixels());
        setPixelColor(0, blue(knobs.brightness), _ring.halfPixels(), CCW);

        setPixelColor(0, white(knobs.brightness), 12+topOffset);
        setPixelColor(0, white(knobs.brightness), 12+topOffset, CCW);
        setPixelColor(_ring.halfPixels()-12, white(knobs.brightness), 12);
        setPixelColor(_ring.halfPixels()-12, white(knobs.brightness), 12, CCW);

        show(200);
    }
}

void Patterns::canadianFlag()
{
    if (isFirstFrame())
    {
        initBrightness();

        uint8_t topOffset = _ring.bottomOffset();

        setPixelColor(0, red(knobs.brightness), _ring.halfPixels());
        setPixelColor(0, red(knobs.brightness), _ring.halfPixels(), CCW);

        setPixelColor(0, white(knobs.brightness), 12+topOffset);
        setPixelColor(0, white(knobs.brightness), 12+topOffset, CCW);
        setPixelColor(_ring.halfPixels()-12, white(knobs.brightness), 12);
        setPixelColor(_ring.halfPixels()-12, white(knobs.brightness), 12, CCW);

        show(200);
    }
}

void Patterns::portugalFlag()
{
    if (isFirstFrame())
    {
        initBrightness();
        
        uint8_t topOffset = _ring.bottomOffset();

        setPixelColor(0, red(knobs.brightness), _ring.halfPixels());
        setPixelColor(0, green(knobs.brightness), _ring.halfPixels(), CCW);

        setPixelColor(0, red(knobs.brightness), 12+topOffset, CCW);
        setPixelColor(_ring.halfPixels()-12, red(knobs.brightness), 12, CCW);

        show(200);
    }
}

void Patterns::rebelFlag()
{
    if (isFirstFrame())
    {
        initBrightness();

        setRingColor(red(knobs.brightness));

        setPixelColor(_ring.topQuarter()/2-6, white(knobs.brightness), _ring.topQuarter()+8, CW, _ring.topQuarter(), 2);
        setPixelColor(_ring.topQuarter()/2-4, blue(knobs.brightness), _ring.topQuarter()+8, CW, _ring.topQuarter(), 8);
        setPixelColor(_ring.topQuarter()/2+4, white(knobs.brightness), _ring.topQuarter()+8, CW, _ring.topQuarter(), 2);

        setPixelColor(_ring.topQuarter()/2-6, white(knobs.brightness), _ring.topQuarter()+8, CCW, _ring.topQuarter(), 2);
        setPixelColor(_ring.topQuarter()/2-4, blue(knobs.brightness), _ring.topQuarter()+8, CCW, _ring.topQuarter(), 8);
        setPixelColor(_ring.topQuarter()/2+4, white(knobs.brightness), _ring.topQuarter()+8, CCW, _ring.topQuarter(), 2);

        show(200);
    }
}


//
//  RAINBOW GROUP
//
void Patterns::rainbowGroup()
{
    rainbowGroup(_pattern);
}

void Patterns::rainbowGroup(uint8_t pattern)
{
    switch (pattern) {
        default:
        case 0:
            rainbowFadeWave();
            break;
        case 1:
            rainbowTheaterWave();
            break;
        case 2:
            rainbowFade();
            break;
        case 3:
            rainbowTheater();
            break;
    }
}

void Patterns::rainbowFadeWave()
{
    initBrightness();
    initSpeed();

    if (isFirstFrame())
        mFrame.color = 0;

    uint8_t c = mFrame.color;
    for (uint16_t i=0; i<_ring.halfPixels(); i++)
    {
        setPixelColor(i, toColor(c, knobs.brightness), 1, CW);
        setPixelColor(i, toColor(c++, knobs.brightness), 1, CCW);
    }

    mFrame.color += 5;

    show(knobs.speed);
}

void Patterns::rainbowFade()
{
    initBrightness();
    initSpeed();

    if (isFirstFrame())
        mFrame.color = 0;

    setRingColor(toColor(mFrame.color++, knobs.brightness));

    show(knobs.speed);
}

void Patterns::rainbowTheaterWave()
{
    initBrightness();
    initSpeed();

    if (isFirstFrame())
    {
        mFrame.color = 0;
        mFrame.offset = 0;
    }

    uint8_t c = mFrame.color;
    for (uint16_t i=0; i<_ring.halfPixels(); i+=3)
    {
        setPixelColor(i+mFrame.offset, toColor(c, knobs.brightness), 1, CW);
        setPixelColor(i+mFrame.offset, toColor(c++, knobs.brightness), 1, CCW);
    }

    show(knobs.speed);
    clear();

    mFrame.offset = inc(mFrame.offset, 3);
    mFrame.color += 5;
}

void Patterns::rainbowTheater()
{
    initBrightness();
    initSpeed();

    if (isFirstFrame())
    {
        mFrame.color = 0;
        mFrame.position = 0;
    }

    setPixelColor(mFrame.position, toColor(mFrame.color, knobs.brightness), _ring.halfPixels(), CW, 3);
    setPixelColor(mFrame.position, toColor(mFrame.color, knobs.brightness), _ring.halfPixels(), CCW, 3);

    show(knobs.speed);
    clear();

    mFrame.position = inc(mFrame.position, 3);
    mFrame.color += 5;
}


//
//  SOLID COLOR GROUP
//
void Patterns::colorGroup()
{
    colorGroup(_pattern);
}

void Patterns::colorGroup(uint8_t pattern)
{
    switch (pattern)
    {
        case 0:
        default:
            onFire(); 
            break;
        case 1: 
            comet(); 
            break;
        case 2: 
            fireFlies(); 
            break;
        case 3: 
            randomPixels(); 
            break;
        case 4:
            flickerColor(); 
            break;
        case 5:
            solidColor(); 
            break;
        case 6:
            starBurst(); 
            break;
    }
}

void Patterns::onFire()
{
    const int sparkAmount = 120;
    const int cooldownAmount = 72;
    const int halfRing = MAX_PIXELS/2;

    static byte heat[halfRing];

    // Step 1.  Cool down every cell a little
    for( uint16_t i=0; i<_ring.halfPixels(); i++) 
    {
        int cooldown = random(0, ((cooldownAmount * 10) / _ring.halfPixels()) + 2);

        if(cooldown > heat[i]) 
            heat[i] = 0;
        else
            heat[i] = heat[i] - cooldown;
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k=_ring.halfPixels() - 1; k >= 2; k--) 
    {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' near the bottom
    if( random(255) < sparkAmount ) 
    {
        int y = random(7);
        heat[y] = heat[y] + random(160, 255);
    }

    // Step 4.  Convert heat to LED colors
    for( uint16_t j=0; j<_ring.halfPixels(); j++) 
    {
        // Scale 'heat' down from 0-255 to 0-191
        byte t192 = map(heat[j], 0, 255, 0, 191);
        
        // calculate ramp up from
        byte heatramp = t192 & 0x3F; // 0..63
        heatramp <<= 2; // scale up to 0..252
        
        // figure out which third of the spectrum we're in:
        if( t192 > 0x80) {                     // hottest
            setPixelColor(_ring.halfPixels()-j-1, yellow(heatramp), 1, DirectionType::CW);
            setPixelColor(_ring.halfPixels()-j-1, yellow(heatramp), 1, DirectionType::CCW);
        } else if( t192 > 0x40 ) {             // middle
            setPixelColor(_ring.halfPixels()-j-1, orange(heatramp), 1, DirectionType::CW);
            setPixelColor(_ring.halfPixels()-j-1, orange(heatramp), 1, DirectionType::CCW);
        } else {                               // coolest
            setPixelColor(_ring.halfPixels()-j-1, red(heatramp), 1, DirectionType::CW);
            setPixelColor(_ring.halfPixels()-j-1, red(heatramp), 1, DirectionType::CCW);
        }
    }

    show(0);
}

void Patterns::comet()
{
    initSpeed();
    initColor();

    if (isFirstFrame())
        mFrame.position = 0;

    // Randomly fade any pixels already displayed.
    for (uint16_t i=0; i<_ring.numPixels(); i++)
    {
        if (random(10) > 5)
            fadeToBlack(i, 0.25);
    }

    // Move the comet forward.
    for (int tail=10; tail>=1; tail--)
    {
        uint16_t lastPixel = loop(mFrame.position, tail, _ring.numPixels());

        setPixelColor(lastPixel, toColor(knobs.color, 255, true));
    }

    show(knobs.speed);

    mFrame.position = inc(mFrame.position, _ring.numPixels());
}

void Patterns::fireFlies()
{
    initSpeed();
    initColor();

    if (isFirstFrame())
    {
        for (uint16_t i=0; i<sizeof(flies)/sizeof(fireFly); i++)
        {
            flies[i].position = random(_ring.numPixels());
            flies[i].brightness = random(1,17);
        }
    }

    for (uint16_t j=0; j<sizeof(flies)/sizeof(fireFly); j++)
    {
        flies[j].brightness = dec(flies[j].brightness, 16);

        if (flies[j].brightness == 0)
        {
            setPixelColor(flies[j].position, black());

            flies[j].position = random(_ring.numPixels());
            flies[j].brightness = 16;
        }

        setPixelColor(flies[j].position, toColor(knobs.color, (flies[j].brightness*flies[j].brightness)-1), true);
    }

    show(knobs.speed);
}

void Patterns::randomPixels()
{
    initBrightness();
    initSpeed();
    initColor();

    for (uint16_t i=0; i<_ring.numPixels(); i++)
        setPixelColor(i, (uint32_t)(random(0, 100) > 70 ? toColor(knobs.color, knobs.brightness, true) : 0));

    show(knobs.speed);
}

void Patterns::flickerColor()
{
    initColor();

    mFrame.brightness = random(0, 255);
    mFrame.speed = random(10, 100);

    if (random(0, mFrame.brightness) < 10)
    {
        setRingColor(toColor(knobs.color, mFrame.brightness, true));

        show(mFrame.speed);
    }
}

void Patterns::solidColor()
{
    initBrightness();
    initSpeed();
    initColor();

    if (twinkle() || knobs.speed < 5)
        setRingColor(toColor(knobs.color, knobs.brightness, true));
    else
        clear();

    show(knobs.speed);
}

void Patterns::starBurst()
{
    initSpeed();
    initColor();

    if (isFirstFrame())
        mFrame.position = 0;

    uint16_t lastPixel;
    for (int tail=16; tail>=1; tail--)
    {
        lastPixel = loop(mFrame.position, tail, _ring.numPixels());

        setPixelColor(lastPixel, toColor(knobs.color, (tail*tail)-1, true));
    }

    show(knobs.speed);

    setPixelColor(lastPixel, black());

    mFrame.position = inc(mFrame.position, _ring.numPixels());
}


//
//  BOUNCE GROUP
//
void Patterns::bounceGroup()
{
    bounceGroup(_pattern);
}

void Patterns::bounceGroup(uint8_t pattern)
{
    switch (pattern) {
        default:
        case 0:
            rainbowNightRider();
            break;
        case 1:
            rainbowQuadRider();
            break;
        case 2:
            colorNightRider();
            break;
        case 3:
            colorQuadRider();
            break;
        case 4:
            bouncingBalls();
            break;
    }
}

void Patterns::rainbowNightRider()
{
    if (isFirstFrame())
        mFrame.color = 0;

    nightRider(mFrame.color);

    mFrame.color += 2;
}

void Patterns::rainbowQuadRider()
{
    if (isFirstFrame())
        mFrame.color = 0;

    quadRider(mFrame.color);

    mFrame.color += 2;
}

void Patterns::colorNightRider()
{
    initColor();
    nightRider(knobs.color);
}

void Patterns::colorQuadRider()
{
    initColor();
    quadRider(knobs.color);
}

void Patterns::nightRider(uint8_t color)
{
    initSpeed();

    static uint16_t lastPixel = _ring.halfPixels() - 1;

    setRingColor(toOppositeColor(color, 9, true));

    for (int i=0; i<mPoints; i++)
    {
        uint8_t brightness = max(((i*i)+1), 4);
        setPixelColor(comet2[i].x, toColor(color, brightness, true), 1);
        setPixelColor(comet2[i].x, toColor(color, brightness, true), 1, CCW);

        bounce(&comet2[i].x, &comet2[i].cw, lastPixel);
    }

    show(knobs.speed);
}

void Patterns::quadRider(uint8_t color)
{
    initSpeed();
    
    static uint16_t lastPixel = _ring.quarterPixels() - 1;

    setRingColor(toOppositeColor(color, 9, true));

    for (int i=0; i<mPoints; i++)
    {
        uint8_t brightness = max(((i*i)+1), 4);
        setPixelColor(comet4[i].x, toColor(color, brightness, true), 1);
        setPixelColor(comet4[i].x, toColor(color, brightness, true), 1, CCW);

        setPixelColor(_ring.halfPixels()-comet4[i].x-1, toColor(color, brightness, true), 1);
        setPixelColor(_ring.halfPixels()-comet4[i].x-1, toColor(color, brightness, true), 1, CCW);

        bounce(&comet4[i].x, &comet4[i].cw, lastPixel);
    }

    show(knobs.speed);
}

void Patterns::bouncingBalls()
{
    const int   startHeight = 1;
    const float gravity = -9.81;
    const float impactVelocityStart = sqrt(-2 * gravity * startHeight);

    uint16_t ballCount = sizeof(balls)/sizeof(bouncingBall);

    initBrightness();
    initColor();

    if (isFirstFrame())
    {
        for (uint16_t i=0; i<ballCount; i++) 
        {  
            balls[i].clockTimeSinceLastBounce = millis();
            balls[i].height = startHeight;
            balls[i].position = 0;
            balls[i].impactVelocity = impactVelocityStart;
            balls[i].timeSinceLastBounce = 0;
            balls[i].dampening = 0.90 - float(i)/pow(ballCount,2);
        }
    }

    for (uint16_t i=0; i<ballCount; i++) 
    {
        balls[i].timeSinceLastBounce =  millis() - balls[i].clockTimeSinceLastBounce;
        balls[i].height = 0.5 * gravity * pow( balls[i].timeSinceLastBounce/1000 , 2.0 ) + balls[i].impactVelocity * balls[i].timeSinceLastBounce/1000;

        if ( balls[i].height < 0 ) 
        {                     
            balls[i].height = 0;
            balls[i].impactVelocity = balls[i].dampening * balls[i].impactVelocity;
            balls[i].clockTimeSinceLastBounce = millis();

            if ( balls[i].impactVelocity < 0.01 ) 
                balls[i].impactVelocity = impactVelocityStart;
        }

        balls[i].position = round(balls[i].height * (_ring.halfPixels() - 2) / startHeight);
    }

    for (uint16_t i=0; i<ballCount; i++) 
    {
        setPixelColor(_ring.halfPixels()-balls[i].position-2, toColor(knobs.color, knobs.brightness, true), 2, DirectionType::CW);
        setPixelColor(_ring.halfPixels()-balls[i].position-2, toColor(knobs.color, knobs.brightness, true), 2, DirectionType::CCW);
    }

    show(10);
    clear();
}

void Patterns::bounce(uint16_t *pos, bool *direction, uint16_t lastPixel)
{
    if (*direction) // Forward
    {
        (*pos)++;
        if ((*pos) > lastPixel)
        {
            (*pos) = lastPixel-1;
            (*direction) = false;
        }
    }
    else    // Backwards
    {
        if ((*pos) == 0)
        {
            (*pos) = 1;
            (*direction) = true;
        }
        else
            (*pos)--;
    }
}


//
//  HOLIDAY LIGHTS GROUP
//
void Patterns::holidayGroup()
{
    holidayGroup(_pattern);
}

void Patterns::holidayGroup(uint8_t pattern)
{
    switch (pattern) {
        default:
        case 0:
            holidaySparkle(); 
            break;
        case 1:
            christmasLights();
            break;
        case 2:
            valentineLights();
            break;
        case 3:
            saintPatrickLights();
            break;
        case 4:
            easterLights();
            break;
        case 5:
            independenceLights();
            break;
        case 6:
            halloweenLights();
            break;
        case 7:
            thanksGivingLights();
            break;
    }
}

void Patterns::holidayLights(uint32_t colors[], uint16_t colorCount)
{
    initSpeed();
    initSize();

    if (isFirstFrame())
        mFrame.offset = 0;

    if (knobs.size > 0)
    {
        // How big does the user want their color segments?
        uint8_t blockSize = map(knobs.size, 1, 254, colorCount, 60) * 2;

        // Make sure we can fit our colors into the segment.        
        blockSize = blockSize - (blockSize % colorCount);

        uint8_t stripeSize = blockSize / colorCount;

        for (uint16_t i=0; i<colorCount; i++)
        {
            setPixelColor(CW, mFrame.offset, _ring.halfPixels(), colors[i], blockSize, stripeSize);
            setPixelColor(CCW, mFrame.offset, _ring.halfPixels(), colors[i], blockSize, stripeSize);
            // setPixelColor(loop(i*segment,ndx,section), colors[i], _ring.halfPixels(), CW, section, segment);
            // setPixelColor(loop(i*segment,ndx,section), colors[i], _ring.halfPixels(), CCW, section, segment);
        }        

        mFrame.offset = inc(mFrame.offset, blockSize);
        
        show(knobs.speed);
    }
    else
    {
        for (uint16_t i=0; i<_ring.numPixels(); i++)
            setPixelColorAbs(i, colors[random(0,colorCount)]);

        show(knobs.speed);
        mFrame.offset = 0;
    }
}

void Patterns::holidaySparkle()
{
    initBrightness();
    initSpeed();
    initColor();

    // Turning the color/size knob will control how many 'black' pixels are included.
    uint8_t color = map(knobs.color, 0, 254, 1, 100);

    for (uint16_t i=0; i<_ring.numPixels(); i++)
        setPixelColor(i, (uint32_t)(random(0, 100) > color ? toColor(random(1, 255), knobs.brightness) : 0));

    show(knobs.speed);
}

void Patterns::christmasLights()
{
    initBrightness();

    uint32_t colors[] =
    {
        green(knobs.brightness),
        orange(knobs.brightness),
        blue(knobs.brightness),
        yellow(knobs.brightness),
        red(knobs.brightness)
    };

    holidayLights(colors, sizeof(colors)/sizeof(uint32_t));
}

void Patterns::valentineLights()
{
    initBrightness();

    uint32_t colors[] =
    {
        red(knobs.brightness),
        white(knobs.brightness/4)
    };

    holidayLights(colors, sizeof(colors)/sizeof(uint32_t));
}

void Patterns::halloweenLights()
{
    initBrightness();

    uint32_t colors[] =
    {
        green(knobs.brightness),
        orange(knobs.brightness),
        white(knobs.brightness/4)
    };

    holidayLights(colors, sizeof(colors)/sizeof(uint32_t));
}

void Patterns::easterLights()
{
    initBrightness();

    uint32_t colors[] =
    {
        yellow(knobs.brightness),
        purple(knobs.brightness),
        white(knobs.brightness/4)
    };

    holidayLights(colors, sizeof(colors)/sizeof(uint32_t));
}

void Patterns::independenceLights()
{
    initBrightness();

    uint32_t colors[] =
    {
        red(knobs.brightness),
        white(knobs.brightness/4),
        blue(knobs.brightness)
    };

    holidayLights(colors, sizeof(colors)/sizeof(uint32_t));
}

void Patterns::saintPatrickLights()
{
    initBrightness();

    uint32_t colors[] =
    {
        green(knobs.brightness),
        white(knobs.brightness/4)
    };

    holidayLights(colors, sizeof(colors)/sizeof(uint32_t));
}

void Patterns::thanksGivingLights()
{
    initBrightness();

    uint32_t colors[] =
    {
        orange(knobs.brightness),
        white(knobs.brightness/4)
    };

    holidayLights(colors, sizeof(colors)/sizeof(uint32_t));
}


//
//  EMERGENCY GROUP
//
void Patterns::emergencyGroup()
{
    emergencyGroup(_pattern);
}

void Patterns::emergencyGroup(uint8_t pattern)
{
    switch (pattern) {
        case 0:
        default:
            chasingPoliceLights();
            break;
        case 1:
            redBlueTripleSegmentFlash();
            break;
        case 2: 
            redBlueHalfRingFlash();
            break;
        case 3: 
            redBlueHalfRingCrawl();
            break;
        case 4:
            redBlueTripleFlash();
            break;
    }
}

void Patterns::redBlueTripleFlash()
{
    initBrightness();
    initSpeed();

    bool toggle = twinkle();
    flash(50, (toggle) ? red(knobs.brightness) : blue(knobs.brightness));
    flash(50, (toggle) ? red(knobs.brightness) : blue(knobs.brightness));
    flash(50, (toggle) ? red(knobs.brightness) : blue(knobs.brightness));

    delay(knobs.speed);
}

void Patterns::redBlueHalfRingFlash()
{
    initBrightness();
    initSpeed();

    clear();

    if (twinkle())
        setPixelColor(0, red(knobs.brightness), _ring.halfPixels());
    else
        setPixelColor(0, blue(knobs.brightness), _ring.halfPixels(), CCW);

    show(knobs.speed);
}

void Patterns::redBlueHalfRingCrawl()
{
    initBrightness();
    initSpeed();
    initSize();

    if (isFirstFrame())
        mFrame.position = 255;

    uint8_t size = map(knobs.size, 0, 254, 4, 20);

    mFrame.position = inc(mFrame.position, _ring.halfPixels());

    clear();

    if (mFrame.position == size)
    {
        // Flash the top section before the crawl...
        for (int i=0; i<3; i++)
        {
            setPixelColor(0, red(knobs.brightness), size);
            setPixelColor(0, blue(knobs.brightness), size, CCW);
            show(50);
            clear();
            show(50);
        }
    }

    setPixelColor(0, red(knobs.brightness), mFrame.position);
    setPixelColor(0, blue(knobs.brightness), mFrame.position, CCW);

    show(knobs.speed);
}

void Patterns::redBlueTripleSegmentFlash()
{
    initBrightness();

    uint32_t colors1[] =
    {
        red(knobs.brightness),
        black()
    };

    uint32_t colors2[] =
    {
        blue(knobs.brightness),
        black()
    };

    if (twinkle())
        chaseLights(colors1, sizeof(colors1)/sizeof(uint32_t));
    else
        chaseLights(colors2, sizeof(colors2)/sizeof(uint32_t));
}

void Patterns::chasingPoliceLights()
{
    initBrightness();

    uint32_t colors[] =
    {
        red(knobs.brightness),
        blue(knobs.brightness),
        black()
    };

    chaseLights(colors, sizeof(colors)/sizeof(uint32_t));
}

void Patterns::chaseLights(uint32_t colors[], uint16_t count)
{
    initSpeed();
    initSize();

    if (isFirstFrame())
        mFrame.position = 0;

    uint8_t section = ((knobs.size >> 4) & 0x000E) + count; // Range: count...30
    section = section - (section % count); // Multiples of count
    uint8_t segment = section / count;

    for (uint16_t i=0; i<count; i++)
    {
        setPixelColor(loop(i*segment,mFrame.position,section), colors[i], _ring.halfPixels(), CW, section, segment);
        setPixelColor(loop(i*segment,mFrame.position,section), colors[i], _ring.halfPixels(), CCW, section, segment);
    }

    mFrame.position = inc(mFrame.position, section);

    show(knobs.speed);
}


//
// CYCLE FAVORITES GROUP
//
void Patterns::cycleGroup()
{
    if (millis() > cycle.timer)
    {
        if (cycle.timer > 0)
            cycle.group = inc(cycle.group, patternGroupType::FLAG_GROUP, patternGroupType::EMERGENCY_GROUP);

        cycle.pattern = _menu.defaultPattern(cycle.group);

        cycle.timer = millis() + 8000; // 8 seconds

        clear(true);
     }

    displayPattern(cycle.group, cycle.pattern);
}


//
// CYCLE ALL PATTERNS GROUP
//
void Patterns::cycleAllGroup()
{
    if (millis() > cycle.timer)
    {
        if (cycle.timer > 0)
        {
            cycle.pattern = inc(cycle.pattern, _patterns->groupPatternCount((patternGroupType)cycle.group)-1);

            if (cycle.pattern == 0)
                cycle.group = inc(cycle.group, patternGroupType::FLAG_GROUP, patternGroupType::EMERGENCY_GROUP+1);
        }

        _menu.restorePattern(cycle.group, cycle.pattern);

        cycle.timer = millis() + 8000; // 8 seconds

        clear(true);
    }

    displayPattern(cycle.group, cycle.pattern);
}


// DISPLAY LED TEST PATTERNS
void Patterns::ledTest()
{
    initSpeed();

    ledTest(knobs.speed);
}

void Patterns::ledTest(uint8_t wait)
{
    for (int i=0; i<8; i++)
    {
        setRingColor(rainbowColor(i,255,true));
        show(wait);
    }

    clear();
    show(wait);
}

// DISPLAY THE BOOT MODE ON THE RING
void Patterns::displayMode(uint8_t mode, unsigned long wait)
{
    // Flash the mode to get attention.
    setPixelColor(0, red(), 4*mode, DirectionType::CW, 4, 2);
    show(50);
    clear();
    show(50);
    setPixelColor(0, red(), 4*mode, DirectionType::CW, 4, 2);
    show(50);
    clear();
    show(50);

    // Now just display the mode solid.
    setPixelColor(0, red(), 4*mode, DirectionType::CW, 4, 2);
    show(wait);
    clear();
}

// THIS IS CALLED TO INITIALIZE THE RING WHEN HOLDING BOTH BUTTONS down
// AND TURNING ON THE UNIT.
uint16_t Patterns::initializeNumPixels(uint8_t value)
{
    uint16_t numPixels = min((value<<1), MAX_PIXELS); // Multiples of 2

    clear();

    for (uint16_t i=0; i<numPixels; i++)
        setPixelColorAbs(i, white(60));

    setPixelColorAbs(numPixels-1, red(60));

    mLeds.show();

    return numPixels;
}

uint8_t Patterns::initializeTopCenter(uint8_t value, uint8_t halfPixels)
{
    uint8_t topCenter = value; // Multiples of 2

    clear();

    setPixelColor(0, red(60), halfPixels);
    setPixelColor(0, green(60), halfPixels, CCW);

    mLeds.show();

    return topCenter;
}

uint8_t Patterns::initializeTopQuarter(uint8_t value)
{
    uint8_t topQuarter = value; // Shrink to half.

    clear();

    setRingColor(yellow(60));

    setPixelColor(0, blue(60), topQuarter);
    setPixelColor(0, blue(60), topQuarter, CCW);

    mLeds.show();

    return topQuarter;
}

void Patterns::setPixelColor(DirectionType dir, uint16_t start, uint32_t color, uint16_t length)
{
    for (uint16_t i=start; i<min(length, _ring.numPixels()); i++)
        setPixelColorAbs(_ring.pixel(i, dir), color);
}

void Patterns::setPixelColor(DirectionType dir, uint16_t start, uint16_t end, uint32_t color, uint16_t blockSize, uint16_t stripeSize)
{
    for (uint16_t i=start; i<end; i+=blockSize)
    {
        for (uint16_t j=i; j<i+stripeSize; j++)
        {
            setPixelColorAbs(_ring.pixel(j % end, dir), color);
        }
    }
}

void Patterns::setPixelColor(uint16_t startPosition, uint32_t color, uint16_t length, DirectionType dir, uint16_t skip, uint16_t litPixels)
{
    // Truncate at the end of our pixel strip if needed.
    uint16_t maxPixels = min(startPosition + length, _ring.numPixels());

    for (uint16_t i=startPosition; i<maxPixels; i+=skip)
    {
        uint16_t colorBarLength = min(i + litPixels, maxPixels);

        for (uint16_t j=i; j<colorBarLength; j++)
        {
            setPixelColorAbs(_ring.pixel(j, dir), color);
        }
    }
}

void Patterns::setRingColor(uint32_t color)
{
    setPixelColor(0, color, _ring.numPixels());
}

void Patterns::flash(uint8_t wait, uint32_t color)
{
    setRingColor(color);
    show(wait);
    clear();
    show(wait);
}

// Input a value 0 to 255 to get a color value. There are 85 transitions
// between each primary color (RGB).
// The colours are a transition r - g - b - back to r.
uint32_t Patterns::colorWheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;

    if (WheelPos <= 85)
        return rgbColor(255 - WheelPos * 3, 0, WheelPos * 3);

    if (WheelPos <= 170)
    {
        WheelPos -= 85;
        return rgbColor(0, WheelPos * 3, 255 - WheelPos * 3);
    }

    WheelPos -= 170;

    return rgbColor(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Input a value 0 to 255 to get a color value. There are 85 transitions
// between each primary color (RGB).
// The colours are a transition r - g - b - back to r.
uint32_t Patterns::colorWheel2(byte WheelPos)
{
    if (WheelPos < 43) // Red to Yellow
        return rgbColor(252, WheelPos * 6, 0);

    if (WheelPos <= 85) // Yellow to Green
    {
        WheelPos -= 43;
        return rgbColor(252 - WheelPos * 6, 252, 0);
    }

    if (WheelPos < 128) // Green to Cyan
    {
        WheelPos -= 85;
        return rgbColor(0, 252, WheelPos * 6);
    }

    if (WheelPos <= 170) // Cyan to Blue
    {
        WheelPos -= 128;
        return rgbColor(0, 252 - WheelPos * 6, 252);
    }

    if (WheelPos < 212) // Blue to Magenta
    {
        WheelPos -= 169;
        return rgbColor(WheelPos * 6, 0, 252);
    }

    WheelPos -= 213; // Magenta to Red

    return rgbColor(252, 0, 252 - WheelPos * 6);
}

uint32_t Patterns::adjustBrightness(uint32_t color, uint8_t brightness)
{
    float scale = brightness / 255.0;

    // Allow for FULL brightness.
    scale = (scale >= 0.95) ? 1.0 : scale;

    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    return rgbColor(r*scale, g*scale, b*scale);
}

uint32_t Patterns::scaleBrightness(uint32_t color, float percentage)
{
    if (color == 0 || (percentage < 0.0 && percentage > 1.0))
        return color;

    float scale = 1.0 - percentage;

    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    return rgbColor(r*scale, g*scale, b*scale);
}

void Patterns::fadeToBlack(uint16_t absolutePos, float percentage) 
{
    mLeds.setPixelColor(absolutePos, scaleBrightness(mLeds.getPixelColor(absolutePos), percentage));
}


/////////////////////////////
// UNUSED CODE - FOR NOW
/////////////////////////////

//
// MORSE CODE GROUP
//
// void Patterns::morseCodeGroup()
// {
//     morseCodeGroup(_pattern);
// }

// void Patterns::morseCodeGroup(uint8_t pattern)
// {
//     switch (pattern) {
//         case 0:
//         default:
//             displayMorse(0, "SOS");
//             break;
//         case 1:
//             displayMorse(1, "SUPER LIGHTS");
//             break;
//         case 2:
//             displayMorse(2, "2.1B");
//             break;
//     }
// }

// void Patterns::showPattern(uint8_t pattern)
// {
//     uint8_t b = (pattern + 1) * 2;
//     setPixelColor(_ring.topQuarter() - b, black(), b, CW, 2);
// }

// void Patterns::dot()
// {
//     setRingColor(toColor(mMorseColor, mMorseBrightness));
//     showPattern(mMorsePattern);
//     show(TIME_UNIT);
//     clear();
//     show(TIME_UNIT);
// }

// void Patterns::dash()
// {
//     setRingColor(toColor(mMorseColor, mMorseBrightness));
//     showPattern(mMorsePattern);
//     show(TIME_UNIT*3);
//     clear();
//     show(TIME_UNIT);
// }

// void Patterns::displayMorse(uint8_t pattern, const char* msg)
// {
//     static int lastIndex = 0;
//     static uint8_t lastPattern = 0;

//     mMorseColor = _menu.currentColor();
//     mMorsePattern = pattern;
//     mMorseBrightness = _menu.currentBrightness();

//     if (lastPattern != pattern)
//     {
//         lastIndex = 0;
//         lastPattern = pattern;
//     }

//     if (msg[lastIndex] == '\0')
//         lastIndex = 0;

//     // short mark, dot or "dit" (▄▄▄▄): "dot duration" is one time unit long
//     // longer mark, dash or "dah" (▄▄▄▄▄▄): three time units long
//     // inter-element gap between the dots and dashes within a character: one dot duration or one unit long
//     // short gap (between letters): three time units long
//     // medium gap (between words): seven time units long

//     char ch = msg[lastIndex++];

//     if (ch == ' ') {
//         delay(TIME_UNIT*7); }

//     else if (ch == '1') {
//         dot(); dash(); dash(); dash(); dash(); delay(TIME_UNIT*2); }
//     else if (ch == '2') {
//         dot(); dot(); dash(); dash(); dash(); delay(TIME_UNIT*2); }
//     else if (ch == '3') {
//         dot(); dot(); dot(); dash(); dash(); delay(TIME_UNIT*2); }
//     else if (ch == '4') {
//         dot(); dot(); dot(); dot(); dash(); delay(TIME_UNIT*2); }
//     else if (ch == '5') {
//         dot(); dot(); dot(); dot(); dot(); delay(TIME_UNIT*2); }
//     else if (ch == '6') {
//         dash(); dot(); dot(); dot(); dot(); delay(TIME_UNIT*2); }
//     else if (ch == '7') {
//         dash(); dash(); dot(); dot(); dot(); delay(TIME_UNIT*2); }
//     else if (ch == '8') {
//         dash(); dash(); dash(); dot(); dot(); delay(TIME_UNIT*2); }
//     else if (ch == '9') {
//         dash(); dash(); dash(); dash(); dot(); delay(TIME_UNIT*2); }
//     else if (ch == '0') {
//         dash(); dash(); dash(); dash(); dash(); delay(TIME_UNIT*2); }

//     else if (ch == 'A') {
//         dot(); dash(); delay(TIME_UNIT*2); }
//     else if (ch == 'B') {
//         dash(); dot(); dot(); dot(); delay(TIME_UNIT*2); }
//     else if (ch == 'C') {
//         dash(); dot(); dash(); dot(); delay(TIME_UNIT*2); }
//     else if (ch == 'D') {
//         dash(); dot(); dot(); delay(TIME_UNIT*2); }
//     else if (ch == 'E') {
//         dot(); delay(TIME_UNIT*2); }
//     else if (ch == 'F') {
//         dot(); dot(); dash(); dot(); delay(TIME_UNIT*2); }
//     else if (ch == 'G') {
//         dash(); dash(); dot(); delay(TIME_UNIT*2); }
//     else if (ch == 'H') {
//         dot(); dot(); dot(); dot(); delay(TIME_UNIT*2); }
//     else if (ch == 'I') {
//         dot(); dot(); delay(TIME_UNIT*2); }
//     else if (ch == 'J') {
//         dot(); dash(); dash(); dash(); delay(TIME_UNIT*2); }
//     else if (ch == 'K') {
//         dash(); dot(); dash(); delay(TIME_UNIT*2); }
//     else if (ch == 'J') {
//         dot(); dash(); dot(); dot(); delay(TIME_UNIT*2); }
//     else if (ch == 'M') {
//         dash(); dash(); delay(TIME_UNIT*2); }
//     else if (ch == 'N') {
//         dash(); dot(); delay(TIME_UNIT*2); }
//     else if (ch == 'O') {
//         dash(); dash(); dash(); delay(TIME_UNIT*2); }
//     else if (ch == 'P') {
//         dot(); dash(); dash(); dot(); delay(TIME_UNIT*2); }
//     else if (ch == 'Q') {
//         dash(); dash(); dot(); dash(); delay(TIME_UNIT*2); }
//     else if (ch == 'R') {
//         dot(); dash(); dot(); delay(TIME_UNIT*2); }
//     else if (ch == 'S') {
//         dot(); dot(); dot(); delay(TIME_UNIT*2); }
//     else if (ch == 'T') {
//         dash(); delay(TIME_UNIT*2); }
//     else if (ch == 'U') {
//         dot(); dot(); dash(); delay(TIME_UNIT*2); }
//     else if (ch == 'V') {
//         dot(); dot(); dot(); dash(); delay(TIME_UNIT*2); }
//     else if (ch == 'W') {
//         dot(); dash(); dash(); delay(TIME_UNIT*2); }
//     else if (ch == 'X') {
//         dash(); dot(); dot(); dash(); delay(TIME_UNIT*2); }
//     else if (ch == 'Y') {
//         dash(); dot(); dash(); dash(); delay(TIME_UNIT*2); }
//     else if (ch == 'Z') {
//         dash(); dash(); dot(); dot(); delay(TIME_UNIT*2); }

//     else if (ch == '.') {
//         dot(); dash(); dot(); dash(); dot(); dash(); delay(TIME_UNIT*2); }

//     if (msg[lastIndex] == '\0')
//         delay(TIME_UNIT*7);
// }

//
//  UTILITY FUNCTIONS
//
// uint16_t Patterns::displayArray(uint16_t startPos, uint16_t size, uint8_t* array)
// {
//     for (uint16_t i=0; i<size/3; i++)
//     {
//         if (startPos >= _ring.numPixels())
//             return startPos;

//         setPixelColorAbs(startPos++, rgbColor(*(array+(i*3)), *(array+(i*3)+1), *(array+(i*3)+2)));
//     }

//     return startPos;
// }

// void Patterns::displayLine(uint16_t size, uint8_t* array, uint8_t wait)
// {
//     uint16_t startPos = 0;
//     while (startPos < _ring.numPixels())
//         startPos = displayArray(startPos, size, array);

//     show(wait);
// }

// void Patterns::displayBitmap(uint16_t lines, uint16_t size, uint8_t* bitmap, uint8_t wait)
// {
//     static uint16_t line = 0;
//     static uint16_t lastLines = 0;

//     if (lines != lastLines)
//     {
//         line = 0;
//         lastLines = lines;
//     }

//     displayLine(size/lines, (bitmap + ((size/lines) * line)), wait);

//     line++;

//     if (line >= lines)
//         line = 0;
// }


// void Patterns::utilityGroup()
// {
//     utilityGroup(_pattern);
// }

// void Patterns::utilityGroup(uint8_t pattern)
// {
//     switch (pattern) {
//         default:
//         case 0: // All LEDs Off
//             allLedsOff();
//             break;
//         case 1: // TEST LEDs
//             ledTest();
//             break;
//         case 2: // SPEED POT
//             potLevels(0);
//             break;
//         case 3: // COLOR POT
//             potLevels(1);
//             break;
//         case 4: // BRIGHT POT
//             potLevels(2);
//             break;
//     }
// }

// void Patterns::allLedsOff()
// {
//     clear();
//     show(_menu.currentSpeed());
// }

// void Patterns::potLevels(uint8_t pattern)
// {
//     clear();

//     switch (pattern) {
//         default:
//         case 0:
//             setPixelColor(0, red(), min(_menu.currentSpeed(), _ring.numPixels()));
//             break;
//         case 1:
//             setPixelColor(0, green(), min(_menu.currentColor(), _ring.numPixels()));
//             break;
//         case 2:
//             setPixelColor(0, blue(), min(_menu.currentBrightness(), _ring.numPixels()));
//             break;
//     }

//     show(50);
// }

// void Patterns::americanFlagFlatTop()
// {
//     uint8_t brightness = _menu.currentBrightness(190);

//     setRingColor(white(brightness));

//     // Blue Panel
//     setPixelColor(12, blue(brightness), _ring.topQuarter()-12, CCW);

//     static uint8_t spacing = (_ring.topQuarter()-12) / 6;

//     // Stars
//     if (twinkle())
//         setPixelColor(spacing-1+12, white(brightness), spacing*4+2, CCW, spacing, 2);
//     else
//         setPixelColor(spacing-1+(spacing/2)+12, white(brightness), spacing*3+2, CCW, spacing, 2);

//     // STRIPS RIGHT
//     uint8_t stripWidth = (_ring.topQuarter()-12) / 7;
//     uint8_t extraPixels = (_ring.topQuarter()-12) % 7;

//     setPixelColor(0, red(brightness), 12, CCW);
//     setPixelColor(0, red(brightness), 12, CW);

//     uint8_t pos = 12, smallWidths = 7 - extraPixels;
//     for (int i=0; i<7; i++)
//     {
//         if (smallWidths > 0)
//         {
//             setPixelColor(pos, (i%2 == 0) ? red(brightness) : white(brightness), stripWidth);
//             pos += stripWidth;
//             smallWidths--;
//         }
//         else
//         {
//             setPixelColor(pos, (i%2 == 0) ? red(brightness) : white(brightness), (stripWidth+1));
//             pos += (stripWidth+1);
//         }
//     }

//     // Strips Bottom
//     stripWidth = (_ring.bottomQuarter()-12) / 6;
//     extraPixels = (_ring.bottomQuarter()-12) % 6;

//     setPixelColor(_ring.topQuarter()+stripWidth, red(brightness), _ring.bottomQuarter()-stripWidth-12, CW, stripWidth*2, stripWidth);
//     setPixelColor(_ring.topQuarter()+stripWidth, red(brightness), _ring.bottomQuarter()-stripWidth-12, CCW, stripWidth*2, stripWidth);

//     setPixelColor(_ring.halfPixels()-12, red(brightness), 12, CCW);
//     setPixelColor(_ring.halfPixels()-12, red(brightness), 12, CW);

//     show(_menu.currentSpeed());
// }
