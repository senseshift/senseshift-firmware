#include "battery/adc_naive.hpp"

#include <Arduino.h>

namespace OH
{
  void ADCNaiveBattery::setup() {
    pinMode(this->pin, INPUT);
  }

  BatteryState ADCNaiveBattery::getValue() {
    return {
      .level = map(analogRead(this->pin), 0.0f, 4095.0f, 0, 255)
    };
  }
} // namespace OH
