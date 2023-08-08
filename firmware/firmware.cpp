#include "senseshift.h"

#if defined(ARDUINO)
#include <Arduino.h>
#endif // ARDUINO

#if defined(SERIAL_PLOTTER) && SERIAL_PLOTTER == true
#include <components/serial_plotter.hpp>
#endif // SERIAL_PLOTTER

#ifndef SERIAL_PLOTTER_PORT
#define SERIAL_PLOTTER_PORT Serial
#endif // SERIAL_PLOTTER_PORT

#ifndef PIO_UNIT_TESTING

extern void setupMode();
extern void loopMode();

#if defined(ARDUINO)

SenseShiftApp App;

void setup()
{
    setupMode();

#if defined(SERIAL_PLOTTER) && SERIAL_PLOTTER == true
    auto* serialOutputState =
      new OH::SerialPlotter_OutputStates<HardwareSerial>(SERIAL_PLOTTER_PORT, App.getHapticBody());
    serialOutputState->begin();
#endif // SERIAL_PLOTTER
}

void loop()
{
    loopMode();
}

#endif // ARDUINO

#endif // PIO_UNIT_TESTING
