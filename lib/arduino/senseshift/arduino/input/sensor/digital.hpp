#pragma once

#include <senseshift/input/sensor.hpp>

#include <Arduino.h>

namespace SenseShift::Arduino::Input {
    typedef ::SenseShift::Input::ISensor<bool> IDigitalSensor;

    template<bool invert = false>
    class DigitalSensor : public IDigitalSensor {
      private:
        uint8_t pin;

      public:
        DigitalSensor(uint8_t pin) : pin(pin) {}

        void setup(void) { pinMode(this->pin, INPUT_PULLUP); };

        bool getValue(void) override;
    };

    template<>
    bool DigitalSensor<false>::getValue(void)
    {
        return digitalRead(this->pin) == LOW;
    }

    template<>
    bool DigitalSensor<true>::getValue(void)
    {
        return digitalRead(this->pin) == HIGH;
    }
} // namespace SenseShift::Arduino::Input
