#pragma once

#include <cstdint>

#include <Arduino.h>

#include <senseshift/input/sensor.hpp>

namespace SenseShift::Arduino::Input {
class DigitalSimpleSensor : public ::SenseShift::Input::IBinarySimpleSensor {
  public:
    explicit DigitalSimpleSensor(
      const std::uint8_t pin, const std::uint8_t mode = INPUT_PULLUP, const std::uint8_t inverted = LOW
    ) :
      pin_(pin), mode_(mode), inverted_(inverted)
    {
    }

    void init() override
    {
        pinMode(this->pin_, this->mode_);
    }

    [[nodiscard]] auto getValue() -> bool override
    {
        return digitalRead(this->pin_) == this->inverted_;
    }

  private:
    std::uint8_t pin_;
    std::uint8_t mode_ = INPUT_PULLUP;
    std::uint8_t inverted_ = LOW;
};
} // namespace SenseShift::Arduino::Input
