#pragma once

#include <senseshift/input/sensor.hpp>

#include <Arduino.h>

namespace SenseShift::Arduino::Input {
    typedef ::SenseShift::Input::ISimpleSensor<bool> IDigitalSensor;

    template<bool invert = false>
    class DigitalSensor : public IDigitalSensor {
      private:
        uint8_t pin;

      public:
        DigitalSensor(uint8_t pin) : pin(pin) {}

        void init() override { pinMode(this->pin, INPUT_PULLUP); };

        bool getValue() override;
    };

    template<>
    bool DigitalSensor<false>::getValue()
    {
        return digitalRead(this->pin) == LOW;
    }

    template<>
    bool DigitalSensor<true>::getValue()
    {
        return digitalRead(this->pin) == HIGH;
    }
} // namespace SenseShift::Arduino::Input
