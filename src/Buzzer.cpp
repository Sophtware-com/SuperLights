#include "Buzzer.h"

Buzzer _buzzer(BUZZER_PIN, 2093); // C7

Buzzer::Buzzer(int pin, int freq)
{
    Pin = pin;
    Freq = freq;

    BuzzerOn = false;
}

void Buzzer::begin()
{
    pinMode(Pin, OUTPUT);
    digitalWrite(Pin, LOW);
}

void Buzzer::beep(int millis)
{
    if (!BuzzerOn)
    {
        // If you don't add the delay after tone call, the buzzer may
        // continue to irradically buzz for a bit.
        tone(Pin, Freq, millis);
        delay(millis);
    }
}

void Buzzer::on()
{
    if (!BuzzerOn)
    {
        tone(Pin, Freq);
        BuzzerOn = true;
    }
}

void Buzzer::off()
{
    noTone(Pin);
    BuzzerOn = false;
}
