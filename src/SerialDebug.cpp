#include <Arduino.h>
#include "SerialDebug.h"

#if defined(USER_SERIAL)

SerialDebug _serialDebug(DEBUG_LEVEL);


SerialDebug::SerialDebug(DebugLevel level, int baud)
{
    mLevel = level;
    mBaud = baud;
}

void SerialDebug::begin()
{
    Serial.begin(mBaud);

    // Wait for port to initialize.
    while (!Serial) ;
}

// DEBUG_

void SerialDebug::debug(const char* msg)
{
    if ((int)mLevel < DebugLevel::debugging)
        return;

    print(msg);
}

void SerialDebug::debugStr(const char* prompt, const char* msg)
{
    if ((int)mLevel < DebugLevel::debugging)
        return;

    printStr(prompt, msg);
}

void SerialDebug::debugInt(const char* prompt, int value, const char* unit)
{
    if ((int)mLevel < DebugLevel::debugging)
        return;

    printInt(prompt, value, unit);
}

void SerialDebug::debugFloat(const char* prompt, float value, const char* unit)
{
    if ((int)mLevel < DebugLevel::debugging)
        return;

    printFloat(prompt, value, unit);
}

// INFO
void SerialDebug::info(const char* msg)
{
    if ((int)mLevel < DebugLevel::info)
        return;

    print(msg);
}

void SerialDebug::infoStr(const char* prompt, const char* msg)
{
    if ((int)mLevel < DebugLevel::info)
        return;

    printStr(prompt, msg);
}

void SerialDebug::infoInt(const char* prompt, int value, const char* unit)
{
    if ((int)mLevel < DebugLevel::info)
        return;

    printInt(prompt, value, unit);
}

void SerialDebug::infoFloat(const char* prompt, float value, const char* unit)
{
    if ((int)mLevel < DebugLevel::info)
        return;

    printFloat(prompt, value, unit);
}

// WARN
void SerialDebug::warn(const char* msg)
{
    if ((int)mLevel < DebugLevel::warning)
        return;

    print(msg);
}

void SerialDebug::warnStr(const char* prompt, const char* msg)
{
    if ((int)mLevel < DebugLevel::warning)
        return;

    printStr(prompt, msg);
}

void SerialDebug::warnInt(const char* prompt, int value, const char* unit)
{
    if ((int)mLevel < DebugLevel::warning)
        return;

    printInt(prompt, value, unit);
}

void SerialDebug::warnFloat(const char* prompt, float value, const char* unit)
{
    if ((int)mLevel < DebugLevel::warning)
        return;

    printFloat(prompt, value, unit);
}

// ERROR
void SerialDebug::error(const char* msg)
{
    if ((int)mLevel < DebugLevel::error)
        return;

    print(msg);
}

void SerialDebug::errorStr(const char* prompt, const char* msg)
{
    if ((int)mLevel < DebugLevel::error)
        return;

    printStr(prompt, msg);
}

void SerialDebug::errorInt(const char* prompt, int value, const char* unit)
{
    if ((int)mLevel < DebugLevel::error)
        return;

    printInt(prompt, value, unit);
}

void SerialDebug::errorFloat(const char* prompt, float value, const char* unit)
{
    if ((int)mLevel < DebugLevel::error)
        return;

    printFloat(prompt, value, unit);
}

void SerialDebug::print(const char* msg)
{
    Serial.println(msg);
}

void SerialDebug::printStr(const char* prompt, const char* msg)
{
    Serial.print(prompt);
    Serial.println(msg);
}

void SerialDebug::printInt(const char* prompt, int value, const char* unit)
{
    Serial.print(prompt);
    Serial.print(": ");
    Serial.print(value);
    Serial.println(unit);
}

void SerialDebug::printFloat(const char* prompt, float value, const char* unit)
{
    Serial.print(prompt);
    Serial.print(": ");
    Serial.print(value);
    Serial.println(unit);
}

#endif // USE_SERIAL
