#include "main.h"

#include <Arduino.h>
#include "openhaptics.h"

#if defined(SERIAL_PLOTTER) && SERIAL_PLOTTER == true
#include <components/serial_plotter.hpp>
#endif

#ifndef PIO_UNIT_TESTING

extern void setupMode(void);

void setup() {
  Serial.begin(115200);
  setupMode();

#if defined(SERIAL_PLOTTER) && SERIAL_PLOTTER == true
  auto* serialOutputState = new OH::SerialPlotter_OutputStates<HardwareSerial>(Serial, App.getOutput());
  App.registerComponent(serialOutputState);
#endif

  App.begin();

  // Free up the Arduino loop task
  vTaskDelete(NULL);
}

void loop() {}

#endif
