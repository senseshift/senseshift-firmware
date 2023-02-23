#include "battery/adc_naive.hpp"

#include <Arduino.h>

void OH::ADCNaiveBattery::setup() {
  pinMode(this->pin, INPUT);
}

uint8_t OH::ADCNaiveBattery::updateLevel() {
  return map(analogRead(this->pin), 0.0f, 4095.0f, 0, 255);
}
