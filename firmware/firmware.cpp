#include "openhaptics.h"

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

#if defined(ARDUINO)

OpenHaptics App;

void setup() {
  setupMode();

#if defined(SERIAL_PLOTTER) && SERIAL_PLOTTER == true
  auto* serialOutputState = new OH::SerialPlotter_OutputStates<HardwareSerial>(SERIAL_PLOTTER_PORT, App.getOutput());
  serialOutputState->begin();
#endif // SERIAL_PLOTTER

  // Free up the Arduino loop task
  vTaskDelete(NULL);
}

void loop() {
  // Arduino loop task is deleted in setup()
  // Delete the `vTaskDelete(NULL);` line in setup() to enable this function
}

#endif // ARDUINO

#endif // PIO_UNIT_TESTING
