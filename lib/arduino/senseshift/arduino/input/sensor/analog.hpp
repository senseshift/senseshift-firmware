#pragma once

#include <senseshift/input/sensor.hpp>

#include <Arduino.h>

namespace SenseShift::Arduino::Input {
    using IAnalogSensor = ::SenseShift::Input::ISimpleSensor<uint16_t>;

    template<bool Invert = false>
    class AnalogSensor : public IAnalogSensor {
        uint8_t pin_;

      public:
        AnalogSensor(const uint8_t pin) : pin_(pin) {}

        void init() override { pinMode(this->pin_, INPUT); };

        [[nodiscard]] auto getValue() -> uint16_t override;
    };

    template<>
    [[nodiscard]] inline auto AnalogSensor<false>::getValue() -> uint16_t
    {
        return analogRead(this->pin_);
    }

    template<>
    [[nodiscard]] inline auto AnalogSensor<true>::getValue() -> uint16_t
    {
        return ANALOG_MAX - analogRead(this->pin_);
    }
} // namespace SenseShift::Arduino::Input
