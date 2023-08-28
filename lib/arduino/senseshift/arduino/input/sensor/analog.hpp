#pragma once

#include <senseshift/input/sensor.hpp>

#include <Arduino.h>

namespace SenseShift::Arduino::Input {
    template<bool invert = false>
    class AnalogSensor : public ::SenseShift::Input::ISimpleSensor<uint16_t> {
      private:
        uint8_t pin;

      public:
        AnalogSensor(uint8_t pin) : pin(pin) {}

        void init() override { pinMode(this->pin, INPUT); };

        uint16_t getValue() override;
    };

    template<>
    uint16_t AnalogSensor<false>::getValue()
    {
        return analogRead(this->pin);
    }

    template<>
    uint16_t AnalogSensor<true>::getValue()
    {
        return ANALOG_MAX - analogRead(this->pin);
    }
} // namespace SenseShift::Arduino::Input
