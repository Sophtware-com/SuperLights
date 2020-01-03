#include "Patterns.h"
#include "Menu.h"


Patterns _patterns(MAX_PIXELS, LED_RING_PIN);


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

void Patterns::setPixelColor(uint16_t relativePos, uint32_t color, uint16_t length, DirectionType dir, uint16_t skip, uint16_t litPixels)
{
    for (uint16_t i=relativePos; i<min((relativePos+length), _ring.numPixels()); i+=skip)
    {
        uint16_t barLength = min((i+litPixels), relativePos+length);

        for (uint16_t j=i; j<min(barLength, _ring.numPixels()); j++)
        {
            uint16_t absPos = _ring.pixel(j, dir);
            setPixelColorAbs(absPos, color);
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
            return 8;
        case HOLIDAY_GROUP:
            return 7;
        case EMERGENCY_GROUP:
            return 4;
        case RAINBOW_GROUP:
            return 6;
        case COLOR_GROUP:
            return 8;
        case CYCLE_GROUP:
            return 1;
        // case MORSE_CODE_GROUP:
        //     return 3;
        // case UTILITY_GROUP:
        //     return 5;
        default:
            return 0;
    }
}

const char* Patterns::groupName(uint8_t group)
{
    static char buffer[20];
    const PROGMEM static char names[][20] =
    {
        "STROBE_GROUP",
        "FLAG_GROUP",
        "HOLIDAY_GROUP",
        "EMERGENCY_GROUP",
        "RAINBOW_GROUP",
        "COLOR_GROUP",
        "CYCLE_GROUP",
        "MORSE_CODE_GROUP",
        "UTILITY_GROUP",
    };

    strcpy_P(buffer, (char*)&(names[group]));

    return buffer;
}

const char* Patterns::patternName(uint8_t group, uint8_t pattern)
{
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
            "americanFlagFlatTop",
            "mexicanFlag",
            "frenchFlag",
            "canadianFlag",
            "rebelFlag",
            "spainFlag",
            "portugalFlag",
            "8",
            "9"
        },
        { // HOLIDAY_GROUP
            "christmasLights",
            "valentineLights",
            "saintPatrickLights",
            "easterLights",
            "independenceLights",
            "halloweenLights",
            "thanksGivingLights",
            "7",
            "8",
            "9"
        },
        { // EMERGENCY_GROUP
            "chasingPoliceLights",
            "redBlueTripleSegmentFlash",
            "redBlueHalfRingFlash",
            "redBlueTripleFlash"
            "4",
            "5",
            "6",
            "7",
            "8",
            "9"
        },
        { // RAINBOW_GROUP
            "rainbowFadeChase",
            "rainbowFade",
            "rainbowChaseFade",
            "rainbowChase",
            "rainbowNightRider",
            "rainbowQuadRider",
            "6",
            "7",
            "8",
            "9"
        },
        { // COLOR_GROUP
            "randomPixels",
            "flickerColor",
            "sparkle",
            "starBurst"
            "nightRider",
            "quadRider",
            "solidColor",
            "solidWhite",
            "8",
            "9"
        },
        { // CYCLE_GROUP
            "CYCLE",
            "1",
            "2",
            "3",
            "4",
            "5"
            "6",
            "7",
            "8",
            "9"
        },
        // { // MORSE_CODE_GROUP
        //     "SOS",
        //     "SUPER LIGHTS",
        //     "VERSION",
        //     "3",
        //     "4",
        //     "5"
        //     "6",
        //     "7",
        //     "8",
        //     "9"
        // },
        // { // UTILITY_GROUP
        //     "allLedsOff",
        //     "ledTest",
        //     "potLevelSpeed",
        //     "potLevelColor",
        //     "potLevelBright",
        //     "5",
        //     "6",
        //     "7",
        //     "8",
        //     "9"
        // },
    };

    strcpy_P(buffer, (char*)&(names[group][pattern]));

    return buffer;
}

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
        case HOLIDAY_GROUP:
            holidayGroup();
            break;
        case EMERGENCY_GROUP:
            emergencyGroup();
            break;
        case RAINBOW_GROUP:
            rainbowGroup();
            break;
        case COLOR_GROUP:
            colorGroup();
            break;
        case CYCLE_GROUP:
            cycleGroup();
            break;
        // case MORSE_CODE_GROUP:
        //     morseCodeGroup();
        //     break;
        // case UTILITY_GROUP:
        //     utilityGroup();
        //     break;
    }
}

void Patterns::cycleGroup()
{
    static int groupIndex = -1;
    static patternGroupType group;
    static uint8_t pattern = 0;
    static unsigned long timer = 0;

    if (millis() > timer)
    {
        groupIndex++;

        if (groupIndex >= 5)
            groupIndex = 0;

        switch (groupIndex) {
            default:
            case 0: group = patternGroupType::FLAG_GROUP; break;
            case 1: group = patternGroupType::HOLIDAY_GROUP; break;
            case 2: group = patternGroupType::EMERGENCY_GROUP; break;
            case 3: group = patternGroupType::RAINBOW_GROUP; break;
            case 4: group = patternGroupType::COLOR_GROUP; break;
        }

        pattern = _menu.defaultPattern(group);

        timer = millis() + 6000;// 10 seconds
    }

    switch (group) {
        case FLAG_GROUP:
            flagGroup(pattern); break;
        case HOLIDAY_GROUP:
            holidayGroup(pattern); break;
        case EMERGENCY_GROUP:
            emergencyGroup(pattern); break;
        case RAINBOW_GROUP:
            rainbowGroup(pattern); break;
        case COLOR_GROUP:
            colorGroup(pattern); break;
        default:
            flagGroup(1); break; // FlatTop flag...
    }
}

void Patterns::utilityGroup()
{
    utilityGroup(_pattern);
}

void Patterns::utilityGroup(uint8_t pattern)
{
    switch (pattern) {
        default:
        case 0: // All LEDs Off
            allLedsOff();
            break;
        case 1: // TEST LEDs
            ledTest();
            break;
        case 2: // SPEED POT
            potLevels(0);
            break;
        case 3: // COLOR POT
            potLevels(1);
            break;
        case 4: // BRIGHT POT
            potLevels(2);
            break;
    }
}

void Patterns::allLedsOff()
{
    clear();
    show(_menu.currentSpeed());
}

void Patterns::potLevels(uint8_t pattern)
{
    clear();

    switch (pattern) {
        default:
        case 0:
            setPixelColor(0, red(), min(_menu.currentSpeed(), _ring.numPixels()));
            break;
        case 1:
            setPixelColor(0, green(), min(_menu.currentColor(), _ring.numPixels()));
            break;
        case 2:
            setPixelColor(0, blue(), min(_menu.currentBrightness(), _ring.numPixels()));
            break;
    }

    show(50);
}

void Patterns::ledTest()
{
    ledTest(_menu.currentSpeed());
}

void Patterns::ledTest(uint8_t wait)
{
    setRingColor(red());
    show(wait);
    setRingColor(green());
    show(wait);
    setRingColor(blue());
    show(wait);
    setRingColor(white());
    show(wait);
    clear();
    show(wait);
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
    uint8_t brightness = _menu.currentBrightness();

    for (int i=0; i<2; i++)
    {
        uint8_t width = max(1,(uint8_t)(((float)_menu.currentColor() * ((float)_ring.numPixels()/254.0)) / 2.0));

        setPixelColor(0, white(brightness), width);
        setPixelColor(0, white(brightness), width, CCW);
        show(50);
        clear();
        show(50);
    }

    delay(800);
}

void Patterns::tripleStrobe()
{
    uint8_t brightness = _menu.currentBrightness();

    for (int i=0; i<3; i++)
    {
        uint8_t width = max(1,(uint8_t)(((float)_menu.currentColor() * ((float)_ring.numPixels()/254.0)) / 2.0));

        setPixelColor(0, white(brightness), width);
        setPixelColor(0, white(brightness), width, CCW);
        show(50);
        clear();
        show(50);
    }

    delay(700);
}

void Patterns::aircraftStrobe()
{
    uint8_t brightness = _menu.currentBrightness();

    uint8_t topOffset = _ring.bottomOffset();

    setPixelColor(0, red(brightness), _ring.halfPixels());
    setPixelColor(0, green(brightness), _ring.halfPixels(), CCW);

    for (int i=0; i<3; i++)
    {
        setPixelColor(0, white(brightness), 12+topOffset);
        setPixelColor(0, white(brightness), 12+topOffset, CCW);
        setPixelColor(_ring.halfPixels()-12, white(brightness), 12);
        setPixelColor(_ring.halfPixels()-12, white(brightness), 12, CCW);
        show(50);
        setPixelColor(0, 0, 12+topOffset);
        setPixelColor(0, 0, 12+topOffset, CCW);
        setPixelColor(_ring.halfPixels()-12, 0, 12);
        setPixelColor(_ring.halfPixels()-12, 0, 12, CCW);
        show(50);
    }

    delay(700);
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
        case 1: // MEXICAN
            americanFlagFlatTop(); break;
        case 2: // MEXICAN
            mexicanFlag(); break;
        case 3: // FRENCH
            frenchFlag(); break;
        case 4: // CANADIAN
            canadianFlag(); break;
        case 5: // REBEL
            rebelFlag(); break;
        case 6: // SPAIN
            spainFlag(); break;
        case 7: // PORTUGAL
            portugalFlag(); break;
    }
}

void Patterns::americanFlag()
{
    uint8_t brightness = _menu.currentBrightness(180);

    setRingColor(white(brightness));

    // Blue Panel
    setPixelColor(0, blue(brightness), _ring.topQuarter(), CCW);

    static uint8_t spacing = _ring.topQuarter() / 6;

    // Stars
    if (twinkle())
        setPixelColor(spacing-1, white(brightness), spacing*4+2, CCW, spacing, 2);
    else
        setPixelColor(spacing-1+(spacing/2), white(brightness), spacing*3+2, CCW, spacing, 2);

    // STRIPS RIGHT
    uint8_t stripWidth = _ring.topQuarter() / 7;
    uint8_t extraPixels = _ring.topQuarter() % 7;

    setPixelColor(0, red(brightness), extraPixels, CW);
    setPixelColor(extraPixels, red(brightness), _ring.topQuarter(), CW, stripWidth*2, stripWidth);

    // Strips Bottom
    stripWidth = _ring.bottomQuarter() / 6;
    extraPixels = _ring.bottomQuarter() % 6;

    setPixelColor(_ring.topQuarter()+stripWidth, red(brightness), _ring.bottomQuarter()-stripWidth, CW, stripWidth*2, stripWidth);
    setPixelColor(_ring.topQuarter()+stripWidth, red(brightness), _ring.bottomQuarter()-stripWidth, CCW, stripWidth*2, stripWidth);

    show(_menu.currentSpeed());
}

void Patterns::americanFlagFlatTop()
{
    uint8_t brightness = _menu.currentBrightness(190);

    setRingColor(white(brightness));

    // Blue Panel
    setPixelColor(12, blue(brightness), _ring.topQuarter()-12, CCW);

    static uint8_t spacing = (_ring.topQuarter()-12) / 6;

    // Stars
    if (twinkle())
        setPixelColor(spacing-1+12, white(brightness), spacing*4+2, CCW, spacing, 2);
    else
        setPixelColor(spacing-1+(spacing/2)+12, white(brightness), spacing*3+2, CCW, spacing, 2);

    // STRIPS RIGHT
    uint8_t stripWidth = (_ring.topQuarter()-12) / 7;
    uint8_t extraPixels = (_ring.topQuarter()-12) % 7;

    setPixelColor(0, red(brightness), 12, CCW);
    setPixelColor(0, red(brightness), 12, CW);

    uint8_t pos = 12, smallWidths = 7 - extraPixels;
    for (int i=0; i<7; i++)
    {
        if (smallWidths > 0)
        {
            setPixelColor(pos, (i%2 == 0) ? red(brightness) : white(brightness), stripWidth);
            pos += stripWidth;
            smallWidths--;
        }
        else
        {
            setPixelColor(pos, (i%2 == 0) ? red(brightness) : white(brightness), (stripWidth+1));
            pos += (stripWidth+1);
        }
    }

    // Strips Bottom
    stripWidth = (_ring.bottomQuarter()-12) / 6;
    extraPixels = (_ring.bottomQuarter()-12) % 6;

    setPixelColor(_ring.topQuarter()+stripWidth, red(brightness), _ring.bottomQuarter()-stripWidth-12, CW, stripWidth*2, stripWidth);
    setPixelColor(_ring.topQuarter()+stripWidth, red(brightness), _ring.bottomQuarter()-stripWidth-12, CCW, stripWidth*2, stripWidth);

    setPixelColor(_ring.halfPixels()-12, red(brightness), 12, CCW);
    setPixelColor(_ring.halfPixels()-12, red(brightness), 12, CW);

    show(_menu.currentSpeed());
}

void Patterns::frenchFlag()
{
    uint8_t brightness = _menu.currentBrightness();

    uint8_t topOffset = _ring.bottomOffset();

    setPixelColor(0, red(brightness), _ring.halfPixels());
    setPixelColor(0, blue(brightness), _ring.halfPixels(), CCW);

    setPixelColor(0, white(brightness), 12+topOffset);
    setPixelColor(0, white(brightness), 12+topOffset, CCW);
    setPixelColor(_ring.halfPixels()-12, white(brightness), 12);
    setPixelColor(_ring.halfPixels()-12, white(brightness), 12, CCW);

    show(_menu.currentSpeed());
}

void Patterns::mexicanFlag()
{
    uint8_t brightness = _menu.currentBrightness();

    uint8_t topOffset = _ring.bottomOffset();

    setPixelColor(0, red(brightness), _ring.halfPixels());
    setPixelColor(0, green(brightness), _ring.halfPixels(), CCW);

    setPixelColor(0, white(brightness), 12+topOffset);
    setPixelColor(0, white(brightness), 12+topOffset, CCW);
    setPixelColor(_ring.halfPixels()-12, white(brightness), 12);
    setPixelColor(_ring.halfPixels()-12, white(brightness), 12, CCW);

    show(_menu.currentSpeed());
}

void Patterns::canadianFlag()
{
    uint8_t brightness = _menu.currentBrightness();

    uint8_t topOffset = _ring.bottomOffset();

    setPixelColor(0, red(brightness), _ring.halfPixels());
    setPixelColor(0, red(brightness), _ring.halfPixels(), CCW);

    setPixelColor(0, white(brightness), 12+topOffset);
    setPixelColor(0, white(brightness), 12+topOffset, CCW);
    setPixelColor(_ring.halfPixels()-12, white(brightness), 12);
    setPixelColor(_ring.halfPixels()-12, white(brightness), 12, CCW);

    show(_menu.currentSpeed());
}

void Patterns::rebelFlag()
{
    uint8_t brightness = _menu.currentBrightness();

    setRingColor(red(brightness));

    setPixelColor(_ring.topQuarter()/2-6, white(brightness), _ring.topQuarter()+8, CW, _ring.topQuarter(), 2);
    setPixelColor(_ring.topQuarter()/2-4, blue(brightness), _ring.topQuarter()+8, CW, _ring.topQuarter(), 8);
    setPixelColor(_ring.topQuarter()/2+4, white(brightness), _ring.topQuarter()+8, CW, _ring.topQuarter(), 2);

    setPixelColor(_ring.topQuarter()/2-6, white(brightness), _ring.topQuarter()+8, CCW, _ring.topQuarter(), 2);
    setPixelColor(_ring.topQuarter()/2-4, blue(brightness), _ring.topQuarter()+8, CCW, _ring.topQuarter(), 8);
    setPixelColor(_ring.topQuarter()/2+4, white(brightness), _ring.topQuarter()+8, CCW, _ring.topQuarter(), 2);

    show(_menu.currentSpeed());
}

void Patterns::spainFlag()
{
    uint8_t brightness = _menu.currentBrightness();

    uint8_t topOffset = _ring.bottomOffset();

    setPixelColor(0, yellow(brightness), _ring.halfPixels());
    setPixelColor(0, yellow(brightness), _ring.halfPixels(), CCW);

    setPixelColor(0, red(brightness), 12+topOffset);
    setPixelColor(0, red(brightness), 12+topOffset, CCW);
    setPixelColor(_ring.halfPixels()-12, red(brightness), 12);
    setPixelColor(_ring.halfPixels()-12, red(brightness), 12, CCW);

    show(_menu.currentSpeed());
}

void Patterns::portugalFlag()
{
    uint8_t brightness = _menu.currentBrightness();

    uint8_t topOffset = _ring.bottomOffset();

    setPixelColor(0, red(brightness), _ring.halfPixels());
    setPixelColor(0, green(brightness), _ring.halfPixels(), CCW);

    setPixelColor(0, red(brightness), 12+topOffset, CCW);
    setPixelColor(_ring.halfPixels()-12, red(brightness), 12, CCW);

    show(_menu.currentSpeed());
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
            rainbowFadeChase();
            break;
        case 1:
            rainbowFade();
            break;
        case 2:
            rainbowChaseFade();
            break;
        case 3:
            rainbowChase();
            break;
        case 4:
            rainbowNightRider();
            break;
        case 5:
            rainbowQuadRider();
            break;
    }
}

void Patterns::rainbowFade()
{
    static uint8_t color = 0;
    uint8_t brightness = _menu.currentBrightness();

    setRingColor(toColor(color++, brightness));

    show(_menu.currentSpeed());
}

void Patterns::rainbowFadeChase()
{
    static uint8_t color = 0;
    uint8_t brightness = _menu.currentBrightness();

    uint8_t c = color;

    for (uint16_t i=0; i<_ring.halfPixels(); i++)
    {
        setPixelColor(i, toColor(c, brightness), 1, CW);
        setPixelColor(i, toColor(c++, brightness), 1, CCW);
    }

    color+=5;

    show(_menu.currentSpeed());
}

void Patterns::rainbowChase()
{
    static int q = 0;
    static uint8_t color = 0;

    uint8_t brightness = _menu.currentBrightness();

    setPixelColor(q, toColor(color, brightness), _ring.halfPixels(), CW, 3);
    setPixelColor(q, toColor(color, brightness), _ring.halfPixels(), CCW, 3);

    show(_menu.currentSpeed());
    clear();

    q = inc(q, 3);
    color+=5;
}

void Patterns::rainbowChaseFade()
{
    static int q = 0;
    static uint8_t color = 0;

    uint8_t brightness = _menu.currentBrightness();

    uint8_t c = color;

    for (uint16_t i=0; i<_ring.halfPixels(); i+=3)
    {
        setPixelColor(i+q, toColor(c, brightness), 1, CW);
        setPixelColor(i+q, toColor(c++, brightness), 1, CCW);
    }

    show(_menu.currentSpeed());
    clear();

    q = inc(q, 3);
    color+=5;
}

void Patterns::rainbowNightRider()
{
    static uint8_t color = 0;

    nightRider(color);
    color+=2;
}

void Patterns::rainbowQuadRider()
{
    static uint8_t color = 0;

    quadRider(color);
    color+=2;
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
        case 0: // RANDOM PIXELS LIT
        default:
            randomPixels(); break;
        case 1: // ALL PIXELS FLICKERING
            flickerColor(); break;
        case 2: // SPARKLE
            sparkle(); break;
        case 3: // STAR BURST
            starBurst(); break;
        case 4: // NIGHT RIDER
            nightRider(); break;
        case 5: // QUAD RIDER
            quadRider(); break;
        case 6: // COLOR KNOB CONTROLLED
            solidColor(); break;
        case 7: // WHITE ONLY
            solidWhite(); break;
    }
}

void Patterns::randomPixels()
{
    uint8_t color = _menu.currentColor();
    uint8_t bright = _menu.currentBrightness();

    for (uint16_t i=0; i<_ring.numPixels(); i++)
        setPixelColor(i, (uint32_t)(random(0, 100) > 50 ? toColor(color, bright) : 0));

    show(_menu.currentSpeed());
}

void Patterns::flickerColor()
{
    uint8_t brightness = random(0, 255);
    if (random(0, brightness) < 10)
    {
        setRingColor(toColor(_menu.currentColor(), brightness));
        show(random(10, 100));
    }
}

void Patterns::sparkle()
{
    for (uint16_t i=0; i<_ring.numPixels(); i++)
        setPixelColor(i, (uint32_t)(random(0, 100) > 50 ? toColor(random(1,255), random(0, 255)) : 0));

    show(_menu.currentSpeed());
}

void Patterns::starBurst()
{
    static uint16_t pos = 0;

    clear();

    uint16_t lastPixel;
    uint8_t color = _menu.currentColor();

    for (int tail=16; tail>=1; tail--)
    {
        lastPixel = loop(pos, tail, _ring.numPixels());
        setPixelColor(lastPixel, toColor(color, (tail*tail)-1));
    }

    show(_menu.currentSpeed());

    setPixelColor(lastPixel, black());

    pos = inc(pos, _ring.numPixels());
}

void Patterns::nightRider()
{
    nightRider(_menu.currentColor());
}

void Patterns::nightRider(uint8_t color)
{
    static uint16_t lastPixel = _ring.halfPixels() - 1;

    setRingColor(toColor(255 - color, 9));

    for (int i=0; i<mPoints; i++)
    {
        uint8_t bright = max(((i*i)+1), 4);
        setPixelColor(comet2[i].x, toColor(color, bright), 1);
        setPixelColor(comet2[i].x, toColor(color, bright), 1, CCW);
        bounce(&comet2[i].x, &comet2[i].cw, lastPixel);
    }

    show(_menu.currentSpeed());
}

void Patterns::quadRider()
{
    quadRider(_menu.currentColor());
}

void Patterns::quadRider(uint8_t color)
{
    static uint16_t lastPixel = _ring.quarterPixels() - 1;

    setRingColor(toColor(255 - color, 9));

    for (int i=0; i<mPoints; i++)
    {
        uint8_t bright = max(((i*i)+1), 4);
        setPixelColor(comet4[i].x, toColor(color, bright), 1);
        setPixelColor(comet4[i].x, toColor(color, bright), 1, CCW);

        setPixelColor(_ring.halfPixels()-comet4[i].x-1, toColor(color, bright), 1);
        setPixelColor(_ring.halfPixels()-comet4[i].x-1, toColor(color, bright), 1, CCW);

        bounce(&comet4[i].x, &comet4[i].cw, lastPixel);
    }

    show(_menu.currentSpeed());
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

void Patterns::solidColor()
{
  if (twinkle() || _menu.currentSpeed() < 5)
      setRingColor(toColor(_menu.currentColor(), _menu.currentBrightness()));
  else
      clear();

  show(_menu.currentSpeed());
}

void Patterns::solidWhite()
{
  if (twinkle() || _menu.currentSpeed() < 5)
      setRingColor(white(_menu.currentBrightness()));
  else
      clear();

  show(_menu.currentSpeed());
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
        case 2: // Pattern 3
            redBlueHalfRingFlash();
            break;
        case 3:
            redBlueTripleFlash();
            break;
    }
}

void Patterns::redBlueTripleFlash()
{
    uint8_t brightness = _menu.currentBrightness();

    bool toggle = twinkle();
    flash(50, (toggle) ? red(brightness) : blue(brightness));
    flash(50, (toggle) ? red(brightness) : blue(brightness));
    flash(50, (toggle) ? red(brightness) : blue(brightness));
    delay(_menu.currentSpeed());
}

void Patterns::redBlueHalfRingFlash()
{
    uint8_t brightness = _menu.currentBrightness();

    clear();

    if (twinkle())
        setPixelColor(0, red(brightness), _ring.halfPixels());
    else
        setPixelColor(0, blue(brightness), _ring.halfPixels(), CCW);

    show(_menu.currentSpeed());
}

void Patterns::redBlueTripleSegmentFlash()
{
    uint8_t brightness = _menu.currentBrightness();

    uint32_t colors1[] =
    {
        red(brightness),
        black()
    };

    uint32_t colors2[] =
    {
        blue(brightness),
        black()
    };

    if (twinkle())
        chaseLights(colors1, sizeof(colors1)/sizeof(uint32_t));
    else
        chaseLights(colors2, sizeof(colors2)/sizeof(uint32_t));
}

void Patterns::chasingPoliceLights()
{
    uint8_t brightness = _menu.currentBrightness();

    uint32_t colors[] =
    {
        red(brightness),
        blue(brightness),
        black()
    };

    chaseLights(colors, sizeof(colors)/sizeof(uint32_t));
}

void Patterns::chaseLights(uint32_t colors[], uint16_t count)
{
    static uint8_t ndx = 0;

    uint8_t section = ((_menu.currentColor() >> 4) & 0x000E) + count; // Range: count...30
    section = section - (section % count); // Multiples of count
    uint8_t segment = section / count;

    for (uint16_t i=0; i<count; i++)
    {
        setPixelColor(loop(i*segment,ndx,section), colors[i], _ring.halfPixels(), CW, section, segment);
        setPixelColor(loop(i*segment,ndx,section), colors[i], _ring.halfPixels(), CCW, section, segment);
    }

    ndx = inc(ndx, section);

    show(_menu.currentSpeed());
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
            christmasLights();
            break;
        case 1:
            valentineLights();
            break;
        case 2:
            saintPatrickLights();
            break;
        case 3:
            easterLights();
            break;
        case 4:
            independenceLights();
            break;
        case 5:
            halloweenLights();
            break;
        case 6:
            thanksGivingLights();
            break;
    }
}

void Patterns::holidayLights(uint32_t colors[], uint16_t count)
{
    static uint8_t ndx = 0;

    uint8_t section = ((_menu.currentColor() >> 4) & 0x000E) + (count*2); // Range: count...(30+count)
    section = section - (section % (count*2)); // Multiples of count * 2
    uint8_t segment = section / count;

    for (uint16_t i=0; i<count; i++)
    {
        setPixelColor(loop(i*segment,ndx,section), colors[i], _ring.halfPixels(), CW, section, segment);
        setPixelColor(loop(i*segment,ndx,section), colors[i], _ring.halfPixels(), CCW, section, segment);
    }

    ndx = inc(ndx, section);

    show(_menu.currentSpeed());
}

void Patterns::christmasLights()
{
    uint8_t brightness = _menu.currentBrightness();

    uint32_t colors[] =
    {
        green(brightness),
        orange(brightness),
        blue(brightness),
        yellow(brightness),
        red(brightness)
    };

    holidayLights(colors, sizeof(colors)/sizeof(uint32_t));
}

void Patterns::halloweenLights()
{
    uint8_t brightness = _menu.currentBrightness();

    uint32_t colors[] =
    {
        green(brightness),
        orange(brightness),
        white(brightness/4)
    };

    holidayLights(colors, sizeof(colors)/sizeof(uint32_t));
}

void Patterns::easterLights()
{
    uint8_t brightness = _menu.currentBrightness();

    uint32_t colors[] =
    {
        yellow(brightness),
        purple(brightness),
        white(brightness/4)
    };

    holidayLights(colors, sizeof(colors)/sizeof(uint32_t));
}

void Patterns::valentineLights()
{
    uint8_t brightness = _menu.currentBrightness();

    uint32_t colors[] =
    {
        red(brightness),
        white(brightness/4)
    };

    holidayLights(colors, sizeof(colors)/sizeof(uint32_t));
}

void Patterns::independenceLights()
{
    uint8_t brightness = _menu.currentBrightness();

    uint32_t colors[] =
    {
        red(brightness),
        white(brightness/4),
        blue(brightness)
    };

    holidayLights(colors, sizeof(colors)/sizeof(uint32_t));
}

void Patterns::saintPatrickLights()
{
    uint8_t brightness = _menu.currentBrightness();

    uint32_t colors[] =
    {
        green(brightness),
        white(brightness/4)
    };

    holidayLights(colors, sizeof(colors)/sizeof(uint32_t));
}

void Patterns::thanksGivingLights()
{
    uint8_t brightness = _menu.currentBrightness();

    uint32_t colors[] =
    {
        orange(brightness),
        white(brightness/4)
    };

    holidayLights(colors, sizeof(colors)/sizeof(uint32_t));
}

//
// MORSE CODE GROUP
//
void Patterns::morseCodeGroup()
{
    morseCodeGroup(_pattern);
}

void Patterns::morseCodeGroup(uint8_t pattern)
{
    switch (pattern) {
        case 0:
        default:
            displayMorse(0, "SOS");
            break;
        case 1:
            displayMorse(1, "SUPER LIGHTS");
            break;
        case 2:
            displayMorse(2, "2.1B");
            break;
    }
}

void Patterns::showPattern(uint8_t pattern)
{
    uint8_t b = (pattern + 1) * 2;
    setPixelColor(_ring.topQuarter() - b, black(), b, CW, 2);
}

void Patterns::dot()
{
    setRingColor(toColor(mMorseColor, mMorseBrightness));
    showPattern(mMorsePattern);
    show(TIME_UNIT);
    clear();
    show(TIME_UNIT);
}

void Patterns::dash()
{
    setRingColor(toColor(mMorseColor, mMorseBrightness));
    showPattern(mMorsePattern);
    show(TIME_UNIT*3);
    clear();
    show(TIME_UNIT);
}

void Patterns::displayMorse(uint8_t pattern, const char* msg)
{
    static int lastIndex = 0;
    static uint8_t lastPattern = 0;

    mMorseColor = _menu.currentColor();
    mMorsePattern = pattern;
    mMorseBrightness = _menu.currentBrightness();

    if (lastPattern != pattern)
    {
        lastIndex = 0;
        lastPattern = pattern;
    }

    if (msg[lastIndex] == '\0')
        lastIndex = 0;

    // short mark, dot or "dit" (▄▄▄▄): "dot duration" is one time unit long
    // longer mark, dash or "dah" (▄▄▄▄▄▄): three time units long
    // inter-element gap between the dots and dashes within a character: one dot duration or one unit long
    // short gap (between letters): three time units long
    // medium gap (between words): seven time units long

    char ch = msg[lastIndex++];

    if (ch == ' ') {
        delay(TIME_UNIT*7); }

    else if (ch == '1') {
        dot(); dash(); dash(); dash(); dash(); delay(TIME_UNIT*2); }
    else if (ch == '2') {
        dot(); dot(); dash(); dash(); dash(); delay(TIME_UNIT*2); }
    else if (ch == '3') {
        dot(); dot(); dot(); dash(); dash(); delay(TIME_UNIT*2); }
    else if (ch == '4') {
        dot(); dot(); dot(); dot(); dash(); delay(TIME_UNIT*2); }
    else if (ch == '5') {
        dot(); dot(); dot(); dot(); dot(); delay(TIME_UNIT*2); }
    else if (ch == '6') {
        dash(); dot(); dot(); dot(); dot(); delay(TIME_UNIT*2); }
    else if (ch == '7') {
        dash(); dash(); dot(); dot(); dot(); delay(TIME_UNIT*2); }
    else if (ch == '8') {
        dash(); dash(); dash(); dot(); dot(); delay(TIME_UNIT*2); }
    else if (ch == '9') {
        dash(); dash(); dash(); dash(); dot(); delay(TIME_UNIT*2); }
    else if (ch == '0') {
        dash(); dash(); dash(); dash(); dash(); delay(TIME_UNIT*2); }

    else if (ch == 'A') {
        dot(); dash(); delay(TIME_UNIT*2); }
    else if (ch == 'B') {
        dash(); dot(); dot(); dot(); delay(TIME_UNIT*2); }
    else if (ch == 'C') {
        dash(); dot(); dash(); dot(); delay(TIME_UNIT*2); }
    else if (ch == 'D') {
        dash(); dot(); dot(); delay(TIME_UNIT*2); }
    else if (ch == 'E') {
        dot(); delay(TIME_UNIT*2); }
    else if (ch == 'F') {
        dot(); dot(); dash(); dot(); delay(TIME_UNIT*2); }
    else if (ch == 'G') {
        dash(); dash(); dot(); delay(TIME_UNIT*2); }
    else if (ch == 'H') {
        dot(); dot(); dot(); dot(); delay(TIME_UNIT*2); }
    else if (ch == 'I') {
        dot(); dot(); delay(TIME_UNIT*2); }
    else if (ch == 'J') {
        dot(); dash(); dash(); dash(); delay(TIME_UNIT*2); }
    else if (ch == 'K') {
        dash(); dot(); dash(); delay(TIME_UNIT*2); }
    else if (ch == 'J') {
        dot(); dash(); dot(); dot(); delay(TIME_UNIT*2); }
    else if (ch == 'M') {
        dash(); dash(); delay(TIME_UNIT*2); }
    else if (ch == 'N') {
        dash(); dot(); delay(TIME_UNIT*2); }
    else if (ch == 'O') {
        dash(); dash(); dash(); delay(TIME_UNIT*2); }
    else if (ch == 'P') {
        dot(); dash(); dash(); dot(); delay(TIME_UNIT*2); }
    else if (ch == 'Q') {
        dash(); dash(); dot(); dash(); delay(TIME_UNIT*2); }
    else if (ch == 'R') {
        dot(); dash(); dot(); delay(TIME_UNIT*2); }
    else if (ch == 'S') {
        dot(); dot(); dot(); delay(TIME_UNIT*2); }
    else if (ch == 'T') {
        dash(); delay(TIME_UNIT*2); }
    else if (ch == 'U') {
        dot(); dot(); dash(); delay(TIME_UNIT*2); }
    else if (ch == 'V') {
        dot(); dot(); dot(); dash(); delay(TIME_UNIT*2); }
    else if (ch == 'W') {
        dot(); dash(); dash(); delay(TIME_UNIT*2); }
    else if (ch == 'X') {
        dash(); dot(); dot(); dash(); delay(TIME_UNIT*2); }
    else if (ch == 'Y') {
        dash(); dot(); dash(); dash(); delay(TIME_UNIT*2); }
    else if (ch == 'Z') {
        dash(); dash(); dot(); dot(); delay(TIME_UNIT*2); }

    else if (ch == '.') {
        dot(); dash(); dot(); dash(); dot(); dash(); delay(TIME_UNIT*2); }

    if (msg[lastIndex] == '\0')
        delay(TIME_UNIT*7);
}

//
//  UTILITY FUNCTIONS
//
uint16_t Patterns::displayArray(uint16_t startPos, uint16_t size, uint8_t* array)
{
    for (uint16_t i=0; i<size/3; i++)
    {
        if (startPos >= _ring.numPixels())
            return startPos;

        setPixelColorAbs(startPos++, rgbColor(*(array+(i*3)), *(array+(i*3)+1), *(array+(i*3)+2)));
    }

    return startPos;
}

void Patterns::displayLine(uint16_t size, uint8_t* array, uint8_t wait)
{
    uint16_t startPos = 0;
    while (startPos < _ring.numPixels())
        startPos = displayArray(startPos, size, array);

    show(wait);
}

void Patterns::displayBitmap(uint16_t lines, uint16_t size, uint8_t* bitmap, uint8_t wait)
{
    static uint16_t line = 0;
    static uint16_t lastLines = 0;

    if (lines != lastLines)
    {
        line = 0;
        lastLines = lines;
    }

    displayLine(size/lines, (bitmap + ((size/lines) * line)), wait);

    line++;

    if (line >= lines)
        line = 0;
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
