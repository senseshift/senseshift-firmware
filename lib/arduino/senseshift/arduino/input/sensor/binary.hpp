#pragma once

#include <senseshift/input/sensor.hpp>

#include <Arduino.h>

namespace SenseShift::Arduino::Input {
    template<bool Invert = false>
    class BinarySensor : public IBinarySensor {
        uint8_t pin_;

      public:
        BinarySensor(const uint8_t pin) : pin_(pin) {}

        void init() override { pinMode(this->pin_, INPUT_PULLUP); };

        [[nodiscard]] auto getValue() -> bool override;
    };

    template<>
    [[nodiscard]] inline auto BinarySensor<false>::getValue() -> bool {
        return digitalRead(this->pin_) == LOW;
    }

    template<>
    [[nodiscard]] inline auto BinarySensor<true>::getValue() -> bool {
        return digitalRead(this->pin_) == HIGH;
    }
} // namespace SenseShift::Arduino::Input
