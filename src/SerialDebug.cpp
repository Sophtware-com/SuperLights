#include <Arduino.h>
#include "SerialDebug.h"


SerialDebug _serialDebug(DEBUG_LEVEL);


SerialDebug::SerialDebug(DebugLevel level, int baud)
{
#if defined(USE_SERIAL)
    mLevel = level;
    mBaud = baud;
#endif
}

void SerialDebug::begin()
{
#if defined(USE_SERIAL)

    Serial.begin(mBaud);

    // Wait for port to initialize.
    while (!Serial) ;

#endif
}

// DEBUG_

void SerialDebug::debug(const char* msg)
{
#if defined(USE_SERIAL)

    if ((int)mLevel < DebugLevel::debugging)
        return;

    print(msg);

#endif
}

void SerialDebug::debugStr(const char* prompt, const char* msg)
{
#if defined(USE_SERIAL)

    if ((int)mLevel < DebugLevel::debugging)
        return;

    printStr(prompt, msg);

#endif
}

void SerialDebug::debugInt(const char* prompt, int value, const char* unit)
{
#if defined(USE_SERIAL)

    if ((int)mLevel < DebugLevel::debugging)
        return;

    printInt(prompt, value, unit);

#endif
}

void SerialDebug::debugFloat(const char* prompt, float value, const char* unit)
{
#if defined(USE_SERIAL)

    if ((int)mLevel < DebugLevel::debugging)
        return;

    printFloat(prompt, value, unit);

#endif
}

// INFO
void SerialDebug::info(const char* msg)
{
#if defined(USE_SERIAL)

    if ((int)mLevel < DebugLevel::info)
        return;

    print(msg);

#endif
}

void SerialDebug::infoStr(const char* prompt, const char* msg)
{
#if defined(USE_SERIAL)

    if ((int)mLevel < DebugLevel::info)
        return;

    printStr(prompt, msg);

#endif
}

void SerialDebug::infoInt(const char* prompt, int value, const char* unit)
{
#if defined(USE_SERIAL)

    if ((int)mLevel < DebugLevel::info)
        return;

    printInt(prompt, value, unit);

#endif
}

void SerialDebug::infoFloat(const char* prompt, float value, const char* unit)
{
#if defined(USE_SERIAL)

    if ((int)mLevel < DebugLevel::info)
        return;

    printFloat(prompt, value, unit);

#endif
}

// WARN
void SerialDebug::warn(const char* msg)
{
#if defined(USE_SERIAL)

    if ((int)mLevel < DebugLevel::warning)
        return;

    print(msg);

#endif
}

void SerialDebug::warnStr(const char* prompt, const char* msg)
{
#if defined(USE_SERIAL)

    if ((int)mLevel < DebugLevel::warning)
        return;

    printStr(prompt, msg);

#endif
}

void SerialDebug::warnInt(const char* prompt, int value, const char* unit)
{
#if defined(USE_SERIAL)

    if ((int)mLevel < DebugLevel::warning)
        return;

    printInt(prompt, value, unit);

#endif
}

void SerialDebug::warnFloat(const char* prompt, float value, const char* unit)
{
#if defined(USE_SERIAL)

    if ((int)mLevel < DebugLevel::warning)
        return;

    printFloat(prompt, value, unit);

#endif
}

// ERROR
void SerialDebug::error(const char* msg)
{
#if defined(USE_SERIAL)

    if ((int)mLevel < DebugLevel::error)
        return;

    print(msg);

#endif
}

void SerialDebug::errorStr(const char* prompt, const char* msg)
{
#if defined(USE_SERIAL)

    if ((int)mLevel < DebugLevel::error)
        return;

    printStr(prompt, msg);

#endif
}

void SerialDebug::errorInt(const char* prompt, int value, const char* unit)
{
#if defined(USE_SERIAL)

    if ((int)mLevel < DebugLevel::error)
        return;

    printInt(prompt, value, unit);

#endif
}

void SerialDebug::errorFloat(const char* prompt, float value, const char* unit)
{
#if defined(USE_SERIAL)

    if ((int)mLevel < DebugLevel::error)
        return;

    printFloat(prompt, value, unit);

#endif
}


#if defined(USE_SERIAL)

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

#endif
