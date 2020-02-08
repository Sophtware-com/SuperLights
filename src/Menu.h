#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include "Pattern.h"

#define MAX_GROUPS 10
#define MAX_PATTERNS 10

#define DEFAULT_OFFSET 16

class Menu
{
private:
    uint16_t mWriteOffset;

    Pattern mLastData;
    uint8_t mLastGroup;
    uint8_t mLastPattern;

public:
    Menu() 
    { 
        mWriteOffset = DEFAULT_OFFSET; 
        mLastGroup = mLastPattern = mLastData.mSpeed = mLastData.mColor = 0; 
    }

    void begin(uint16_t writeOffset = DEFAULT_OFFSET);
    bool isInitialized();
    void writeDefaults();

    //void writeGroupSpeedColor(uint8_t group, uint8_t speed, uint8_t color);
    void writePatternSpeedColor(uint8_t group, uint8_t pattern, uint8_t speed, uint8_t color);

    void dumpPatterns(); // Inits Serial

    uint8_t lastGroup() { return mLastGroup; }
    void lastGroup(uint8_t group) { mLastGroup = group; }
    uint8_t lastPattern() { return mLastPattern; }
    void lastPattern(uint8_t pattern) { mLastPattern = pattern; }

    bool groupChanged();
    bool patternChanged();

    //void writeLastMenu();
    void writeLastGroup();
    void writeLastPattern();
    void writeLastPatternData();

    //uint8_t readLastGroup();
    uint8_t readLastPattern();
    Pattern readLastPatternData();

    void updateLastGroup(bool displayLastPattern);
    void updateLastPattern();

    void restorePattern(uint8_t group, uint8_t pattern);
    uint8_t defaultPattern(uint8_t group);

    uint8_t currentSpeed();
    uint8_t currentColor();
    uint8_t currentBrightness();

protected:
    uint16_t lastGroupOffset();
    uint16_t strobeBrightOffset();
    uint16_t navBrightOffset();
    uint16_t groupOffset(uint8_t group);
    uint16_t patternOffset(uint8_t pattern, uint8_t group);
    uint16_t patternSpeedOffset(uint16_t patternOffset);
    uint16_t patternColorOffset(uint16_t patternOffset);
};

extern Menu _menu;

extern volatile uint8_t _group;
extern volatile uint8_t _pattern;

#endif //MENU_H
