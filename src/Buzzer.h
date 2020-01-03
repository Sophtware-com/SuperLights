#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

#define BUZZER_PIN 9

class Buzzer
{
private:

    int Pin;
    int Freq;

    bool BuzzerOn;

public:
    Buzzer(int pin, int freq);

    void begin();

    void beep(int millis = 100);

    void on();

    void off();
};

extern Buzzer _buzzer;

#endif // BUZZER_H
