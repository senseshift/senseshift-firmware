#include "battery/adc_naive.hpp"

#include <utility.hpp>
#include <Arduino.h>

namespace OH
{
  void ADCNaiveBattery::setup() {
    pinMode(this->pin, INPUT);
  }

  BatteryState ADCNaiveBattery::getValue() {
    return {
      .level = simpleMap<uint16_t>(analogRead(this->pin), 4095, 255)
    };
  }
} // namespace OH
