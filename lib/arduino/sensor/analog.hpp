#pragma once

#include <sensor.hpp>

#include <Arduino.h>

namespace OH
{
  template <bool invert = false>
  class AnalogSensor : public ISensor<uint16_t>
  {
   private:
    uint8_t pin;

   public:
    AnalogSensor(uint8_t pin) : pin(pin) {}

    void setup(void)
    {
      pinMode(this->pin, INPUT);
    };

    uint16_t getValue(void) override;
  };

  template <>
  uint16_t AnalogSensor<false>::getValue(void)
  {
    return analogRead(this->pin);
  }

  template <>
  uint16_t AnalogSensor<true>::getValue(void)
  {
    return ANALOG_MAX - analogRead(this->pin);
  }
} // namespace OH
