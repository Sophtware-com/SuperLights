#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

#define BUZZER_PIN 6

class Buzzer
{
private:

    int Pin;
    int Freq;

    bool BuzzerOn;

public:
    Buzzer(int pin, int freq)
    {
        Pin = pin;
        Freq = freq;

        BuzzerOn = false;
    }

    void begin()
    {
        pinMode(Pin, OUTPUT);
        digitalWrite(Pin, LOW);
    }

    void beep(int millis=100)
    {
        tone(Pin, Freq);
        delay(millis);
        noTone(Pin);
    }

    // void Buzzer::on()
    // {
    //     if (!BuzzerOn)
    //     {
    //         tone(Pin, Freq);
    //         BuzzerOn = true;
    //     }
    // }

    // void Buzzer::off()
    // {
    //     noTone(Pin);
    //     BuzzerOn = false;
    // }
};

extern Buzzer _buzzer;

#endif // BUZZER_H
