#include <Arduino.h>
#include <avr/sleep.h>

#include "Global.h"
#include "RingConfig.h"
#include "Patterns.h"
#include "SensorRead.h"
#include "Buzzer.h"
#include "Menu.h"

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

// MODES, ?=not used, X=value, lowercase=no push
#define MODE_NORMAL       0 // pg-???
#define MODE_INITIALIZE   1 // PG-LLL
#define MODE_RANDOM       2 // Pg-?X?, where X is between 3 and 15 seconds
#define MODE_FAVORITES    3 // pG-?X?, where X is between 3 and 15 seconds
#define MODE_CYCLE_LONG   4 // PG-RRR
#define MODE_CYCLE_SHORT  5 // PG-RRL
#define MODE_CONTINUOUS   6 // PG-RLR
#define MODE_STROBES      7 // PG-LRL

uint8_t _mode = MODE_NORMAL;


// PROTOTYPES
void groupButtonRisingISR();
void groupButtonFallingISR();
void patternButtonRisingISR();
void patternButtonFallingISR();

uint8_t nextGroup()
{
    static bool paused = false;

    if (_mode == MODE_CONTINUOUS)
        _pattern = 0;

    if (_mode == MODE_FAVORITES )
    {
        if (_group == patternGroupType::CYCLE_GROUP)
        {
            paused = _pattern == 1;
            return (uint8_t)patternGroupType::STROBE_GROUP;
        }

        _pattern = paused ? 1 : 0;
        return (uint8_t)patternGroupType::CYCLE_GROUP;
    }

    if (_mode == MODE_CYCLE_LONG || _mode == MODE_CYCLE_SHORT)
    {
        if (_group == patternGroupType::CYCLE_ALL_GROUP)
        {
            paused = _pattern == 1;
            return (uint8_t)patternGroupType::STROBE_GROUP;
        }

        _pattern = paused ? 1 : 0;
        return (uint8_t)patternGroupType::CYCLE_ALL_GROUP;
    }

    if (_mode == MODE_STROBES)
        return (uint8_t)(patternGroupType::STROBE_GROUP);

    if (_mode == MODE_RANDOM)
    {
        if (_group == patternGroupType::RANDOM_PATTERN_GROUP)
        {
            paused = _pattern == 1;
            return (uint8_t)patternGroupType::STROBE_GROUP;
        }

        _pattern = paused ? 1 : 0;
        return (uint8_t)patternGroupType::RANDOM_PATTERN_GROUP;
    }

    return (_group < (uint8_t)patternGroupType::EMERGENCY_GROUP) ? _group + 1 : 0;
}

uint8_t nextPattern()
{
    if (_mode == MODE_CONTINUOUS)
    {
        uint8_t p = (_pattern == _patterns.groupPatternCount((patternGroupType)_group)-1) ? 0 : _pattern + 1;

        // Here we would have normally just looped to the start of this group, but in this mode
        // we skip to the next group so the pattern button moves through ALL patterns. You can
        // still use the group button to skip groups though.
        if (p == 0)
            _group = nextGroup();

        return p;
    }

    if (_mode == MODE_FAVORITES  || _mode == MODE_CYCLE_LONG || _mode == MODE_RANDOM)
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
            if (_mode == MODE_CONTINUOUS)
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
    }

    _speed.begin();
    _color.begin();
    _bright.begin();

    _ringConfig.begin();
    _ring.begin();

    int brightPos = _bright.knobPosition();
    int speedPos = _speed.knobPosition();
    int colorPos = _color.knobPosition();

    // Set the MODE here.
    if (bothButtonsHeld())
    {
        if (brightPos == POS_LEFT && speedPos == POS_LEFT && colorPos == POS_LEFT)         // PG-LLL
        {
            _ringConfig.init();
            _ring.begin();
        }
        else if (brightPos == POS_RIGHT && speedPos == POS_LEFT && colorPos == POS_RIGHT)  // PG-RLR
        {
            _mode = MODE_CONTINUOUS;
        }
        else if (brightPos == POS_RIGHT && speedPos == POS_RIGHT && colorPos == POS_RIGHT) // PG-RRR
        {
            _mode = MODE_CYCLE_LONG;
            _cycleDelayMS = (300000 / _patterns.patternCount()) - (27 * _patterns.patternCount()); // All patterns in 5 minutes, minus 30ms transition time.
        }
        else if (brightPos == POS_RIGHT && speedPos == POS_RIGHT && colorPos == POS_LEFT)  // PG-RRL
        {
            _mode = MODE_CYCLE_SHORT;
            _cycleDelayMS = (120000 / _patterns.patternCount()) - (27 * _patterns.patternCount()); // All patterns in 2 minutes, minus 30ms transition time.
        }
        else if (brightPos == POS_LEFT && speedPos == POS_RIGHT && colorPos == POS_LEFT)   // PG-LRL
        {
            _mode = MODE_STROBES;
        }
    }
    else if (digitalRead(GROUP_BUTTON_PIN) == LOW)    // pG-?X?, where X is between 3 and 15 seconds
    {
        _mode = MODE_FAVORITES ;
        _cycleDelayMS = map(speedPos, 0, _speed.maxValue(), 3, 15) * 1000;
    } 
    else if (digitalRead(PATTERN_BUTTON_PIN) == LOW) // Pg-?X?, where X is between 3 and 15 seconds
    {
        _mode = MODE_RANDOM;
        _cycleDelayMS = map(speedPos, 0, _speed.maxValue(), 3, 15) * 1000;
    }

    _menu.begin();

    // Force a mode and delay here if needed for testing...
    //_mode = MODE_CYCLE_LONG;
    //_cycleDelayMS = 3000;

    // In these modes the pattern button is used to pause and unpause the lights,
    // and we start at the flag group.
    if (_mode == MODE_RANDOM)
    {
        _group = (uint8_t)patternGroupType::RANDOM_PATTERN_GROUP;
        _pattern = 0;
    }

    if (_mode == MODE_FAVORITES )
    {
        _group = (uint8_t)patternGroupType::CYCLE_GROUP;
        _pattern = 0;
    }

    if (_mode == MODE_CYCLE_LONG || _mode == MODE_CYCLE_SHORT)
    {
        _group = (uint8_t)patternGroupType::CYCLE_ALL_GROUP;
        _pattern = 0;
    }

    if (_mode == MODE_STROBES)
    {
        _group = (uint8_t)patternGroupType::STROBE_GROUP;
        _pattern = _menu.defaultPattern(_group);
    }

    // Display the MODE on the ring as a series of dashes.
    if (_mode > MODE_NORMAL)
    {
        _buzzer.beep();
        _patterns.displayMode(_mode, 3000);
        delay(200);
    }

    // Attach the button interrupts.
    attachInterrupt(digitalPinToInterrupt(GROUP_BUTTON_PIN), groupButtonFallingISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(PATTERN_BUTTON_PIN), patternButtonFallingISR, FALLING);

    _groupButtonPushedTime = 0;
    _patternButtonPushedTime = 0;

    _buzzer.beep();
    _patterns.ledTest(255);
    _patterns.clear(true);

    _menu.restorePattern(_group, _pattern);
}

void loop()
{
    if (_groupButtonPushedTime != 0 && !_groupButtonBeep && millis() - _groupButtonPushedTime > BUTTON_LONG_PUSH)
    {
        _groupButtonBeep = true;
        _buzzer.beep();

        _menu.dumpPatterns();
    }

    if (_patternButtonPushedTime != 0 && !_patternButtonBeep && millis() - _patternButtonPushedTime > BUTTON_LONG_PUSH)
    {
        _patternButtonBeep = true;
        _buzzer.beep();
    }

    if (_menu.groupChanged())
    {
        _buzzer.beep();

        // Save the current pattern data for the item we're leaving.
        _menu.writeLastPatternData();

        // Update the 'Last' vars with the new values.
        _menu.updateLastGroup(_mode == MODE_NORMAL || _mode == MODE_CONTINUOUS || _mode == MODE_STROBES);

        // Only update the last group when in NORMAL mode.
        if (_mode == MODE_NORMAL)
            _menu.writeLastGroup();

        _patterns.clear(true);
    }

    if (_menu.patternChanged())
    {
        _buzzer.beep();

        _menu.writeLastPatternData();

        _menu.updateLastPattern();

        // Only save the last pattern when in normal mode.
        if (_mode == MODE_NORMAL)
            _menu.writeLastPattern();

        _patterns.clear(true);
    }

    // Default config.
    // _ringConfig.eraseEEPROM();
    // _ringConfig.writeDefaults();
    // _menu.writeDefaults();
    // while (1) ;

    _patterns.displayPattern();
}
