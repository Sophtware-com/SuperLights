#ifndef PATTERNS_H
#define PATTERNS_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "SerialDebug.h"
#include "Menu.h"
#include "Ring.h"

#define LED_RING_PIN 7
#define LED_STAR_PIN 4

#define TO_WHITE 253 // toColor/toRainbow/rainbowColor functions will change to white after this color.

#define CYCLE_DELAY_SECONDS 8 // How long each pattern displays in a cycle mode.

//#define TIME_UNIT 120

enum patternGroupType
{
    STROBE_GROUP,
    FLAG_GROUP,
    RAINBOW_GROUP,
    COLOR_GROUP,
    BOUNCE_GROUP,
    HOLIDAY_GROUP,
    EMERGENCY_GROUP,
    CYCLE_GROUP,
    CYCLE_ALL_GROUP,
    NUMBER_OF_GROUPS
};


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type FLAGS_GROUP, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

struct cometPoint
{
    uint16_t x;
    bool cw;
};

struct bouncingBall
{
    float height;
    float impactVelocity;
    float timeSinceLastBounce;
    int   position;
    long  clockTimeSinceLastBounce;
    float dampening;
};

struct knobPositions
{
    uint8_t brightness;
    uint8_t speed;
    union
    {
        uint8_t color;
        uint8_t size;
    };
};

struct cyclePosition
{
    uint8_t group;
    uint8_t pattern;
    unsigned long timer;
};

struct patternFrame
{
    uint8_t maxFrames;
    uint8_t currentFrame;

    uint16_t position;
    uint16_t offset;

    uint8_t brightness;
    uint8_t speed;
    union
    {
        uint8_t color;
        uint8_t size;
    };
};

struct fireFly
{
    uint8_t position;
    uint8_t brightness;
};

class Patterns
{
    Adafruit_NeoPixel mLeds;

    bool mTwinkle = false;
    
    // This is set from the main loop when a pattern changes. This gives
    // the pattern a chance to run one-time initialization.
    bool mInit = true;

    // uint8_t mMorseColor = 0;
    // uint8_t mMorseBrightness = 0;
    // uint8_t mMorsePattern = 0;

    uint8_t mPoints;
    cometPoint comet2[16];
    cometPoint comet4[16];

    bouncingBall balls[4];

    knobPositions knobs;

    cyclePosition cycle;

    patternFrame mFrame;

    fireFly flies[16];

public:
    Patterns(uint16_t ledCount, uint8_t pin) : mLeds(ledCount, pin, NEO_GRB + NEO_KHZ800)
    {
        mLeds.begin();

        initComet();

        cycle.group = patternGroupType::FLAG_GROUP;
        cycle.pattern = 0;
        cycle.timer = 0;
    };

    Adafruit_NeoPixel& getLeds() { return mLeds; }

    uint8_t* getPixels() { return mLeds.getPixels(); }
    uint16_t getPixelBytes() { return mLeds.numPixels() * 3; }

    void initComet()
    {
        mPoints = sizeof(comet2)/sizeof(cometPoint);

        for (int i=0; i<mPoints; i++)
        {
            comet2[i].x = i;
            comet2[i].cw = true;

            comet4[i].x = i;
            comet4[i].cw = true;
        }
    }

    inline void initBrightness() 
    { 
        knobs.brightness = _menu.currentBrightness(); 
    }

    inline void initSpeed() 
    { 
        knobs.speed = _menu.currentSpeed(); 
    }

    inline void initColor() 
    { 
        knobs.color = _menu.currentColor(); 
    }

    inline void initSize() 
    { 
        knobs.size = _menu.currentColor(); 
    }

    inline void resetFirstFrame() { mInit = true; }
    bool isFirstFrame() { bool firstFrame = mInit; if (firstFrame) mInit = !mInit; return firstFrame; }

    // HELPERS FOR THE MENU SYSTEM IN LOOP.
    const uint8_t maxGroups();
    const uint8_t maxGroupPatterns();
    const uint8_t groupPatternCount(patternGroupType pattern);

    const char* groupName(uint8_t group);
    const char* patternName(uint8_t group, uint8_t pattern);

    void displayPattern();
    void displayPattern(uint8_t group);
    void displayPattern(uint8_t group, uint8_t pattern);


    // NeoPixel shortcuts
    inline void clear(bool init=false) { mLeds.clear(); mInit = init; }
    inline void show(unsigned long wait = 0) { mLeds.show(); delay(wait); }
    inline uint32_t rgbColor(uint8_t r, uint8_t g, uint8_t b) { return mLeds.Color(r,g,b); };
    inline void setBrightness(uint8_t brightness) { mLeds.setBrightness(brightness); }
 
    inline void setPixelColorAbs(uint16_t absolutePos, uint32_t color) { mLeds.setPixelColor(absolutePos, color); }
    
    void setPixelColor(uint32_t color, uint16_t pos, uint16_t len=1, DirectionType dir=DirectionType::CW, uint16_t skipLen=1, uint16_t pixelLen=1, bool isEnd=false);
    
//    void setPixelColor(uint16_t relativePos, uint32_t color, uint16_t length=1, DirectionType dir=DirectionType::CW, uint16_t skip=1, uint16_t litPixels=1);
    
    void setRingColor(uint32_t color);
    
    void flash(uint8_t wait, uint32_t color);

    bool twinkle() { mTwinkle = !mTwinkle; return mTwinkle; }

    // Returns a color from the color wheel based on index from 0..255
    uint32_t colorWheel(byte WheelPos);
    uint32_t colorWheel2(byte WheelPos);

    // Takes a color wheel index and brightness and returns the color.
    uint32_t toColor(uint8_t colorIndex, uint8_t brightness, bool useWhite=false) { 
        return (useWhite && colorIndex > TO_WHITE) ? white(brightness) : adjustBrightness(colorWheel(colorIndex), brightness); }
    uint32_t toOppositeColor(uint8_t colorIndex, uint8_t brightness, bool useWhite=false) { 
        return (useWhite && colorIndex > TO_WHITE) ? black() : adjustBrightness(colorWheel(255-colorIndex), brightness); }
    uint32_t toRainbow(uint8_t colorIndex, uint8_t brightness, bool useWhite=false) {
        return rainbowColor(map(colorIndex,0,254,0,(useWhite) ? 7 : 6), brightness); }
    uint32_t rainbowColor(uint8_t colorIndex, uint8_t brightness) {
        switch (colorIndex) {
            default:
            case 0: return red(brightness);
            case 1: return orange(brightness);
            case 2: return yellow(brightness);
            case 3: return green(brightness);
            case 4: return blue(brightness);
            case 5: return indigo(brightness);
            case 6: return violet(brightness);
            case 7: return white(brightness);
        }
    }
    uint32_t toPrimary(uint8_t colorIndex, uint8_t brightness, bool useWhite=false) {
        return primaryColor(map(colorIndex, 0, 254, 0, (useWhite) ? 3 : 2), brightness);
    }
    uint32_t primaryColor(uint8_t colorIndex, uint8_t brightness) {
        switch (colorIndex) {
            default:
            case 0: return red(brightness);
            case 1: return green(brightness);
            case 2: return blue(brightness);
            case 3: return white(brightness);
        }
    }

    // Takes a color a uniformly adjusts RGB based on brightness scale 0..255
    uint32_t adjustBrightness(uint32_t color, uint8_t brightness);

    // Takes a color and scales RGB brightness based on brightness scale 0..255
    uint32_t scaleBrightness(uint32_t color, float percentage);

    // Takes an existing pixel and fades the color (brightness) towards black.
    void fadeToBlack(uint16_t absolutePos, float percentage);

    // BASIC COLORS
    inline uint32_t black() { return 0; }
    inline uint32_t white(uint8_t brightness=70) { return adjustBrightness(mLeds.Color(255,255,255), brightness); }

    // Color helpers.
    inline uint32_t red(uint8_t brightness=255) { return adjustBrightness(mLeds.Color(255,0,0), brightness); }
    inline uint32_t orange(uint8_t brightness=255) { return toColor(11, brightness); }
    inline uint32_t yellow(uint8_t brightness=255) { return adjustBrightness(mLeds.Color(255,255,0), brightness); }
    inline uint32_t green(uint8_t brightness=255) { return adjustBrightness(mLeds.Color(0,255,0), brightness); }
    inline uint32_t blue(uint8_t brightness=255) { return adjustBrightness(mLeds.Color(0,0,255), brightness); }
    inline uint32_t indigo(uint8_t brightness=255) { return toColor(223, brightness); }
    inline uint32_t violet(uint8_t brightness=255) { return adjustBrightness(mLeds.Color(255,0,255), brightness); }

    // Odd-ball color helpers.
    inline uint32_t cyan(uint8_t brightness=255) { return adjustBrightness(mLeds.Color(0,255,255), brightness); }
    inline uint32_t purple(uint8_t brightness=255) { return toColor(223, brightness); }
    inline uint32_t magenta(uint8_t brightness=255) { return adjustBrightness(mLeds.Color(255,0,255), brightness); }

    // Helpers
    // MIN is inclusive, while MAX is exclusive.
    inline uint16_t inc(uint16_t n, uint16_t max) { return n == (max-1) ? 0 : n+1; }
    inline uint16_t inc(uint16_t n, uint16_t min, uint16_t max) { return n == (max-1) ? min : n+1; }
    inline uint16_t dec(uint16_t n, uint16_t max) { return n == 0 ? max-1 : n-1; }
    inline uint16_t dec(uint16_t n, uint16_t min, uint16_t max) { return n == min ? max-1 : n-1; }
  
    inline uint16_t loop(uint16_t a, uint16_t b, uint16_t max) { return (a + b) % max; }
    void bounce(uint16_t *pos, bool *direction, uint16_t lastPixel);

    inline uint16_t nextFrame() { mFrame.currentFrame = inc(mFrame.currentFrame, 1, mFrame.maxFrames+1); return mFrame.currentFrame; }

    // MIN and MAX are included in the results (inclusive).
    inline uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    // MIN and MAX are included in the results (inclusive).
    inline float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }


    // INITIALIZATION ROUTINES
    uint16_t initializeNumPixels(uint8_t value);
    uint8_t initializeTopCenter(uint8_t value, uint8_t halfPixels);
    uint8_t initializeTopQuarter(uint8_t value);

    // STROBE_GROUP
    void strobeGroup();
    void strobeGroup(uint8_t pattern);
    void doubleStrobe();
    void tripleStrobe();
    void aircraftStrobe();
    void landingLights();

    // FLAG_GROUP
    void flagGroup();
    void flagGroup(uint8_t pattern);
    void americanFlag();
    void spainFlag();
    void mexicanFlag();
    void frenchFlag();
    void canadianFlag();
    void portugalFlag();
    void rebelFlag();

    // RAINBOW_GROUP
    void rainbowGroup();
    void rainbowGroup(uint8_t pattern);
    void rainbowFadeWave();
    void rainbowFade();
    void rainbowTheaterWave();
    void rainbowTheater();
    void gayPride();

    // COLOR_GROUP
    void colorGroup();
    void colorGroup(uint8_t pattern);
    void onFire();
    void comet();
    void fireFlies();
    void randomPixels();
    void flickerColor();
    void starBurst();
    void solidColor();

    // BOUNCE_GROUP
    void bounceGroup();
    void bounceGroup(uint8_t pattern);
    void rainbowNightRider();
    void rainbowQuadRider();
    void colorNightRider();
    void colorQuadRider();
    void bouncingBalls();
    void nightRider(uint8_t color, bool useWhite=false);
    void quadRider(uint8_t color, bool useWhite=false);

    // HOLIDAY_GROUP
    void holidayGroup();
    void holidayGroup(uint8_t pattern);
    void holidaySparkle();
    void christmasLights();
    void halloweenLights();
    void easterLights();
    void valentineLights();
    void independenceLights();
    void saintPatrickLights();
    void thanksGivingLights();
    void hanukkah();
    void ramadan();

    // EMERGENCY_GROUP
    void emergencyGroup();
    void emergencyGroup(uint8_t pattern);
    void redBlueTripleFlash();
    void redBlueTripleSegmentFlash();
    void redBlueHalfRingFlash();
    void redBlueHalfRingCrawl();
    void chasingPoliceLights();

    // CYCLE_GROUP
    void cycleGroup();

    // CYCLE_ALL_GROUP
    void cycleAllGroup();

    void colorWipe(uint8_t wait, uint8_t brightness, uint8_t color);
    void theaterChase(uint8_t wait, uint8_t brightness, uint8_t color);
    void stripedLights(uint32_t colors[], uint16_t count);

    void ledTest();
    void ledTest(uint8_t wait);
    void displayInit();
    void displayMode(uint8_t mode, unsigned long wait);


    // Morse code functions.
    // void showPattern(uint8_t pattern);
    // void dot();
    // void dash();
    // void displayMorse(uint8_t pattern, const char* msg);
    // void morseCodeGroup();
    // void morseCodeGroup(uint8_t pattern);

    // UTILITY_GROUP
    // void utilityGroup();
    // void utilityGroup(uint8_t pattern);
    // void allLedsOff();
    // void potLevels(uint8_t pattern);

    // Display ring from a bitmap.
    // uint16_t displayArray(uint16_t startPos, uint16_t size, uint8_t* array);
    // void displayLine(uint16_t size, uint8_t* array, uint8_t wait);
    // void displayBitmap(uint16_t lines, uint16_t size, uint8_t* bitmap, uint8_t wait);
};

extern Patterns _patterns;

#endif // PATTERNS_H
