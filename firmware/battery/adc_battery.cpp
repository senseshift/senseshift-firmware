#include "battery/adc_battery.h"

#include <Arduino.h>

uint8_t ADCBattery::updateLevel() {
  return map(analogRead(this->pin), 0.0f, 4095.0f, 0, 255);
}
