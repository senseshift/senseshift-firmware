#include "main.h"

#include <Arduino.h>
#include "openhaptics.h"

#ifdef SERIAL_PLOTTER
#include "components/serial_plotter.h"
#endif

#ifndef PIO_UNIT_TESTING

extern void setupMode(void);

void setup() {
  Serial.begin(115200);
  setupMode();

#ifdef SERIAL_PLOTTER
  auto* serialOutputState = new SerialPlotter_OutputStates<HardwareSerial>(Serial);
  App.registerComponent(serialOutputState);
#endif

  App.begin();

  // Free up the Arduino loop task
  vTaskDelete(NULL);
}

void loop() {}

#endif
