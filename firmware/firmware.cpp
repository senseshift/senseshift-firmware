#include "senseshift.h"

#if defined(ARDUINO)
#include <Arduino.h>
#endif // ARDUINO

#if defined(SENSESHIFT_SERIAL_PLOTTER) && SENSESHIFT_SERIAL_PLOTTER == true
#include <senseshift/arduino/components/serial_plotter.hpp>
#endif // SENSESHIFT_SERIAL_PLOTTER

#ifndef SENSESHIFT_SERIAL_PLOTTER_PORT
#define SENSESHIFT_SERIAL_PLOTTER_PORT Serial
#endif // SENSESHIFT_SERIAL_PLOTTER_PORT

#ifndef PIO_UNIT_TESTING

extern void setupMode();
extern void loopMode();

#if defined(ARDUINO)

SenseShift::Application App;

void setup()
{
    setupMode();

#if defined(SENSESHIFT_SERIAL_PLOTTER) && SENSESHIFT_SERIAL_PLOTTER == true
    auto* serialOutputState = new ::SenseShift::Arduino::SerialPlotter_OutputStates<HardwareSerial>(
      SENSESHIFT_SERIAL_PLOTTER_PORT,
      App.getHapticBody()
    );
    serialOutputState->begin();
#endif // SENSESHIFT_SERIAL_PLOTTER
}

void loop()
{
    loopMode();
}

#endif // ARDUINO

#endif // PIO_UNIT_TESTING
