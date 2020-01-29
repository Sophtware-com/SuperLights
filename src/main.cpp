#include <Arduino.h>
#include <avr/sleep.h>

#include "Global.h"
#include "RingConfig.h"
#include "Patterns.h"
#include "SensorRead.h"
#include "Buzzer.h"
#include "Menu.h"

const char* VERSION_NO = "Rev 4.0.01";

// PIN ASSIGNMENTS
#define PWR_LED_PIN   8
#define CHRG_LED_PIN  11
#define STDBY_LED_PIN 12

#define CHRG_STAT_PIN  9 
#define STDBY_STAT_PIN 10

#define  PWR_EN_PIN  4 // VLED

#define  SUPPLY_TYPE 13 // This is also the LED pin on the UNO.

// BUTTON MENU
const uint16_t BUTTON_LONG_PUSH  = 2000; // Over this

volatile bool _groupButtonBeep = false;
volatile unsigned long _groupButtonPushedTime = 0;
volatile bool _patternButtonBeep = false;
volatile unsigned long _patternButtonPushedTime = 0;

// MODES
#define MODE_0_INIT       0
#define MODE_1_NORMAL     1
#define MODE_2_CONTINUOUS 2
#define MODE_3_FAVORTITE  3
#define MODE_4_LOOPING    4
#define MODE_5_STOBES     5

uint8_t _mode = MODE_1_NORMAL;

// PROTOTYPES
void groupButtonRisingISR();
void groupButtonFallingISR();
void patternButtonRisingISR();
void patternButtonFallingISR();

uint8_t nextGroup()
{
    if (_mode == MODE_2_CONTINUOUS)
        _pattern = 0;

    if (_mode == MODE_3_FAVORTITE)
        return (uint8_t)(_group == patternGroupType::STROBE_GROUP) ? patternGroupType::CYCLE_GROUP : patternGroupType::STROBE_GROUP;

    if (_mode == MODE_4_LOOPING)
        return (uint8_t)(_group == patternGroupType::STROBE_GROUP) ? patternGroupType::CYCLE_ALL_GROUP : patternGroupType::STROBE_GROUP;

    if (_mode == MODE_5_STOBES)
        return (uint8_t)(patternGroupType::STROBE_GROUP);

    // We do -2 to skip the cycle groups in normal mode.
    return (_group < (uint8_t)patternGroupType::EMERGENCY_GROUP) ? _group + 1 : 0;
}

uint8_t nextPattern()
{
    if (_mode == MODE_2_CONTINUOUS)
    {
        uint8_t p = (_pattern == _patterns.groupPatternCount((patternGroupType)_group)-1) ? 0 : _pattern + 1;

        // Here we would have normally just looped to the start of this group, but in this mode
        // we skip to the next group so the pattern button moves through ALL patterns. You can
        // still use the group button to skip groups though.
        if (p == 0)
            _group = nextGroup();

        return p;
    }

    if (_mode == MODE_3_FAVORTITE || _mode == MODE_4_LOOPING)
        return (_pattern == 1) ? 0 : 1;

    return (_pattern == _patterns.groupPatternCount((patternGroupType)_group)-1) ? 0 : _pattern + 1;
}

// ISR's
void groupButtonFallingISR()
{
    // Catch the time when the button was pushed.
    _groupButtonPushedTime = millis();

    // Attach interrupt to catch when the button is released.
    attachInterrupt(digitalPinToInterrupt(GROUP_BUTTON_PIN), groupButtonRisingISR, RISING);
}

void groupButtonRisingISR()
{
    if (_groupButtonPushedTime != 0)
    {
        unsigned long buttonReleasedTime = millis();

        // Quick push
        if (buttonReleasedTime - _groupButtonPushedTime < BUTTON_LONG_PUSH)
        {
            _group = nextGroup();
        }
        else  // 2s buton push resets back to 0.
        {
            _group = 0;

            // Also loop the patterns back to the beginning when resetting group.
            if (_mode == MODE_2_CONTINUOUS)
                _pattern = 0;
        }

        _groupButtonBeep = false;
        _groupButtonPushedTime = 0;
    }

    attachInterrupt(digitalPinToInterrupt(GROUP_BUTTON_PIN), groupButtonFallingISR, FALLING);
}

void patternButtonFallingISR()
{
    _patternButtonPushedTime = millis();
    attachInterrupt(digitalPinToInterrupt(PATTERN_BUTTON_PIN), patternButtonRisingISR, RISING);
}

void patternButtonRisingISR()
{
    if (_patternButtonPushedTime != 0)
    {
        unsigned long buttonReleasedTime = millis();

        if (buttonReleasedTime - _patternButtonPushedTime < BUTTON_LONG_PUSH)
        {
            _pattern = nextPattern();
        }
        else
        {
            _pattern = 0;
        }

        _patternButtonBeep = false;
        _patternButtonPushedTime = 0;
    }

    attachInterrupt(digitalPinToInterrupt(PATTERN_BUTTON_PIN), patternButtonFallingISR, FALLING);
}


bool bothButtonsHeld()
{
    return digitalRead(GROUP_BUTTON_PIN) == LOW && digitalRead(PATTERN_BUTTON_PIN) == LOW;
}

void setup()
{
    // Initialize pins:
    pinMode(PWR_LED_PIN, OUTPUT);
    pinMode(CHRG_LED_PIN, OUTPUT);
    pinMode(STDBY_LED_PIN, OUTPUT);
    pinMode(CHRG_STAT_PIN, INPUT);
    pinMode(STDBY_STAT_PIN, INPUT);

    // initialize the pushbutton pins as an input:
    pinMode(GROUP_BUTTON_PIN, INPUT);
    pinMode(PATTERN_BUTTON_PIN, INPUT);

    pinMode(PWR_EN_PIN, INPUT);

    _serialDebug.begin();
    _serialDebug.info("SUPER LED Lights");
    _serialDebug.info(VERSION_NO);
    _serialDebug.info();

    _buzzer.begin();

    // Are we in CHARGING mode...
    if (digitalRead(PWR_EN_PIN) == LOW)
    {
        digitalWrite(PWR_LED_PIN, LOW);
        _buzzer.beep();

        // Monitor the charging functions and report to the RGB
        // LED the status of the charging. Blue=charging, Green=done.
        while (1)
        {
            // The status pin are held high with pullup resistors and
            // are pulled low with that charging function is enabled.
            digitalWrite(CHRG_LED_PIN, digitalRead(CHRG_STAT_PIN) ? LOW : HIGH);
            digitalWrite(STDBY_LED_PIN, digitalRead(STDBY_STAT_PIN) ? LOW : HIGH);
            delay(1000);
        }
    }
    else
    {
        digitalWrite(PWR_LED_PIN, HIGH);
        _buzzer.beep();
    }

    _speed.begin();
    _color.begin();
    _bright.begin();

    _ringConfig.begin();
    if (!_ringConfig.isInitialized())
    {
        _ringConfig.init();
        _mode = MODE_1_NORMAL;
    }

    // Set the MODE here.
    if (bothButtonsHeld())
    {
        int brightPos = _bright.knobPosition();
        int speedPos = _speed.knobPosition();
        int colorPos = _color.knobPosition();

        // NOTE: SPEED KNOB IS FLIPPED IN SOFTWARE

        if (brightPos == POS_LEFT && speedPos == POS_LEFT && colorPos == POS_LEFT)          // LLL
        {
            _ringConfig.init();
            _mode = MODE_1_NORMAL;
        }
        else if (brightPos == POS_RIGHT && speedPos == POS_LEFT && colorPos == POS_LEFT)    // RLL
            _mode = MODE_2_CONTINUOUS;
         else if (brightPos == POS_RIGHT && speedPos == POS_RIGHT && colorPos == POS_LEFT)  // RRL
            _mode = MODE_3_FAVORTITE;
         else if (brightPos == POS_RIGHT && speedPos == POS_LEFT && colorPos == POS_RIGHT)  // RLR
            _mode = MODE_4_LOOPING;
         else if (brightPos == POS_RIGHT && speedPos == POS_RIGHT && colorPos == POS_RIGHT) // RRR
            _mode = MODE_5_STOBES;

        _patterns.displayMode(_mode, 3000);
        delay(200);
     }

    // Readjust the LED buffer to the exact length needed.
    _patterns.getLeds().updateLength(_ringConfig.numPixels());

    _patterns.ledTest(255);

    _menu.begin();

    // In these modes the pattern button is used to pause and unpause the lights,
    // and we start at the flag group.
    if (_mode == MODE_3_FAVORTITE)
    {
        _group = (uint8_t)patternGroupType::CYCLE_GROUP;
        _pattern = 0;

        _menu.lastGroup(_group);
        _menu.lastPattern(_pattern);
    }

    if (_mode == MODE_4_LOOPING)
    {
        _group = (uint8_t)patternGroupType::CYCLE_ALL_GROUP;
        _pattern = 0;

        _menu.lastGroup(_group);
        _menu.lastPattern(_pattern);
    }

    if (_mode == MODE_5_STOBES)
    {
        _group = (uint8_t)patternGroupType::STROBE_GROUP;
        _pattern = _menu.defaultPattern(_group);

        _menu.lastGroup(_group);
        _menu.lastPattern(_pattern);
    }

    _serialDebug.infoInt("Mode:", _mode);
    _serialDebug.info();

    // Attach the button interrupts.
    attachInterrupt(digitalPinToInterrupt(GROUP_BUTTON_PIN), groupButtonFallingISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(PATTERN_BUTTON_PIN), patternButtonFallingISR, FALLING);

    _groupButtonPushedTime = 0;
    _patternButtonPushedTime = 0;

    _buzzer.beep();
}

void loop()
{
    if (_groupButtonPushedTime != 0 && !_groupButtonBeep && millis() - _groupButtonPushedTime > BUTTON_LONG_PUSH)
    {
        _groupButtonBeep = true;
        _buzzer.beep();
    }

    if (_patternButtonPushedTime != 0 && !_patternButtonBeep && millis() - _patternButtonPushedTime > BUTTON_LONG_PUSH)
    {
        _patternButtonBeep = true;
        _buzzer.beep();
    }

    if (_menu.groupChanged())
    {
        _buzzer.beep();

        _serialDebug.info();
        _serialDebug.info(_patterns.groupName(_menu.lastGroup()));
        _serialDebug.info(_patterns.groupName(_group));

        // Save the current pattern data for the item we're leaving.
        _menu.writeLastPatternData();

        // Update the 'Last' vars with the new values.
        _menu.updateLastGroup(_mode != MODE_2_CONTINUOUS);

        // Only update the last group when in NORMAL mode.
        if (_mode == MODE_1_NORMAL)
            _menu.writeLastGroup();

        _patterns.clear(true);
    }

    if (_menu.patternChanged())
    {
        _buzzer.beep();

        _serialDebug.info();
        _serialDebug.info(_patterns.patternName(_group, _pattern));

        _menu.writeLastPatternData();

        _menu.updateLastPattern();

        if (_mode == MODE_1_NORMAL)
            _menu.writeLastPattern();

        _patterns.clear(true);
    }

    _patterns.displayPattern();
}
