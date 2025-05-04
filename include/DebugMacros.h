#ifndef DEBUG_MACROS_H
#define DEBUG_MACROS_H

#include <Arduino.h>

extern const bool DEBUG;

// Macros para depuração
#define DEBUG_PRINT(x)         \
    do {                       \
        if (DEBUG) {           \
            Serial.print(x);   \
        }                      \
    } while (0)

#define DEBUG_PRINTLN(x)       \
    do {                       \
        if (DEBUG) {           \
            Serial.println(x); \
        }                      \
    } while (0)

#endif