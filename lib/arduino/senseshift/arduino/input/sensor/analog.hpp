#pragma once

#include <cstdint>

#include <senseshift/input/sensor.hpp>

#include <Arduino.h>

namespace SenseShift::Arduino::Input {
    template<bool Invert = false>
    class AnalogSensor : public ::SenseShift::Input::IFloatSensor {
        std::uint8_t pin_;

      public:
        AnalogSensor(const std::uint8_t pin) : pin_(pin) {}

        void init() override { pinMode(this->pin_, INPUT); };

        [[nodiscard]] auto getValue() -> float override;
    };

    template<>
    [[nodiscard]] inline auto AnalogSensor<false>::getValue() -> float
    {
        const auto raw = analogRead(this->pin_);
        return static_cast<float>(raw) / ANALOG_MAX;
    }

    template<>
    [[nodiscard]] inline auto AnalogSensor<true>::getValue() -> float
    {
        const auto raw = ANALOG_MAX - analogRead(this->pin_);
        return static_cast<float>(raw) / ANALOG_MAX;
    }
} // namespace SenseShift::Arduino::Input
