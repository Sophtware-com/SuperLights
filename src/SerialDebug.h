#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

//#define USE_SERIAL

enum DebugLevel
{
    none,       // Not used for writing messages.
    debugging,  // Used for interactive investigation during development.
    info,       // Track the general flow of the application.
    warning,    // Highlight an abnormal or unexpected event.
    error,      // Highlight when the current flow of execution is stopped.
    critical,   // Describe an unrecoverable application or system crash.
    trace       // Contain the most detailed messages.
};

// Define the debug level for the entire application.
// If you're on a Leonardo and NOT using serial, undefine USE_SERIAL in
// the SerialDebug.h file. Then SerialDebug will output nothing.
#define DEBUG_LEVEL DebugLevel::info

class SerialDebug
{
private:

#if defined(USE_SERIAL)
    int mBaud;
    DebugLevel mLevel;
#endif

public:

    SerialDebug(DebugLevel level, int baud = 9600);

    void begin();

    // DEBUG_

    void debug(const char* msg = "");
    void debugStr(const char* prompt, const char* msg = "");
    void debugInt(const char* prompt, int value, const char* unit = "");
    void debugFloat(const char* prompt, float value, const char* unit = "");

    // INFO
    void info(const char* msg = "");
    void infoStr(const char* prompt, const char* msg = "");
    void infoInt(const char* prompt, int value, const char* unit = "");
    void infoFloat(const char* prompt, float value, const char* unit = "");

    // WARN
    void warn(const char* msg = "");
    void warnStr(const char* prompt, const char* msg = "");
    void warnInt(const char* prompt, int value, const char* unit = "");
    void warnFloat(const char* prompt, float value, const char* unit = "");

    // ERROR
    void error(const char* msg = "");
    void errorStr(const char* prompt, const char* msg = "");
    void errorInt(const char* prompt, int value, const char* unit = "");
    void errorFloat(const char* prompt, float value, const char* unit = "");


protected:

#if defined(USE_SERIAL)
    void print(const char* msg = "");
    void printStr(const char* prompt, const char* msg = "");
    void printInt(const char* prompt, int value, const char* unit = "");
    void printFloat(const char* prompt, float value, const char* unit = "");
#endif
};

// Make the debug object available to all.
extern SerialDebug _serialDebug;

#endif // DEBUG_H
