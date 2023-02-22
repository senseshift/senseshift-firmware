#include "battery/adc_battery.h"

#include <Arduino.h>

void ADCBattery::setup() {
  pinMode(this->pin, INPUT);
}

uint8_t ADCBattery::updateLevel() {
  return map(analogRead(this->pin), 0.0f, 4095.0f, 0, 255);
}
