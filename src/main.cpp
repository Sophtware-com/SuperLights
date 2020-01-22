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

// MODES
#define MODE_0_INIT       0
#define MODE_1_NORMAL     1
#define MODE_2_CONTINUOUS 2
#define MODE_3_FAVORTIE   3
#define MODE_4_ALL        4
unsigned int _mode = MODE_1_NORMAL;

// PROTOTYPES
void groupButtonRisingISR();
void groupButtonFallingISR();
void patternButtonRisingISR();
void patternButtonFallingISR();

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
            _group = (_group == _patterns.maxGroups()-1) ? 0 : _group + 1;
        }
        else  // 2s buton push resets back to 0.
        {
            _group = 0;
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
            _pattern = (_pattern == _patterns.groupPatternCount((patternGroupType)_group)-1) ? 0 : _pattern + 1;
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

    pinMode(PWR_EN_PIN, INPUT);

    _serialDebug.begin();
    _serialDebug.info("SUPER LED Lights");
    _serialDebug.info("Rev 4.0");
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
    if (!_ringConfig.isInitialized() || bothButtonsHeld())
    {
        _ringConfig.init();
        delay(200); // Give the user time to release the button.
    }

    // Readjust the LED buffer to the exact length needed.
    _patterns.getLeds().updateLength(_ringConfig.numPixels());

    _patterns.ledTest(255);

    _menu.begin();

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
        _menu.updateLastGroup();

        _menu.writeLastGroup();

        _patterns.clear();
    }

    if (_menu.patternChanged())
    {
        _buzzer.beep();

        _serialDebug.info();
        _serialDebug.info(_patterns.patternName(_group, _pattern));

        _menu.writeLastPatternData();

        _menu.updateLastPattern();

        _menu.writeLastPattern();

        _patterns.clear();
    }

    _patterns.displayPattern();
}
