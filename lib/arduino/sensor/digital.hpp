#pragma once

#include <sensor.hpp>
#include <Arduino.h>

namespace OH
{
  typedef ISensor<bool> IDigitalSensor;

  template <bool invert = false>
  class DigitalSensor : public IDigitalSensor
  {
   private:
    uint8_t pin;
    uint8_t _pinMode;

   public:
    DigitalSensor(uint8_t pin, uint8_t pinMode) : pin(pin), _pinMode(pinMode) {
      // static_assert(
      //   _pinMode == INPUT || _pinMode == INPUT_PULLUP || _pinMode == INPUT_PULLDOWN,
      //   "pinMode must be INPUT, INPUT_PULLUP or INPUT_PULLDOWN"
      // );
    }

    void setup(void)
    {
      pinMode(this->pin, this->_pinMode);
    };

    bool getValue(void) override;
  };

  template <>
  bool DigitalSensor<false>::getValue(void)
  {
    return digitalRead(this->pin) == HIGH;
  }

  template <>
  bool DigitalSensor<true>::getValue(void)
  {
    return digitalRead(this->pin) == LOW;
  }
} // namespace OH
