#include "openhaptics.h"

#if defined(ARDUINO)
#include <Arduino.h>
#endif // ARDUINO

#if defined(SERIAL_PLOTTER) && SERIAL_PLOTTER == true
#include <components/serial_plotter.hpp>
#endif // SERIAL_PLOTTER

#ifndef PIO_UNIT_TESTING

extern void setupMode(OpenHaptics* app);

#if defined(ARDUINO)

static OpenHaptics App;

void setup() {
  Serial.begin(115200);
  setupMode(&App);

#if defined(SERIAL_PLOTTER) && SERIAL_PLOTTER == true
  auto* serialOutputState = new OH::SerialPlotter_OutputStates<HardwareSerial>(Serial, App.getOutput());
  App.registerComponent(serialOutputState);
#endif // SERIAL_PLOTTER

  App.begin();

  // Free up the Arduino loop task
  vTaskDelete(NULL);
}

void loop() {}

#endif // ARDUINO

#endif // PIO_UNIT_TESTING
