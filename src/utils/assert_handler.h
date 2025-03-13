#ifndef ASSERT_HANDLER
#include <Arduino.h>
#include <stdint.h>

#define ASSERT(exp, msg) \
    do { \
        if (exp) { \
            digitalWrite(A5, HIGH);    \
            Serial.println(msg);       \
        } \
    } while(0)


#endif