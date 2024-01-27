#include "senseshift.h"

#if defined(ARDUINO)
#include <Arduino.h>
#endif // ARDUINO

#ifndef PIO_UNIT_TESTING

extern void setupMode();
extern void loopMode();

#if defined(ARDUINO)

SenseShift::Application App;

void setup()
{
    setupMode();
}

void loop()
{
    loopMode();
}

#endif // ARDUINO

#endif // PIO_UNIT_TESTING
