#pragma once

#include <abstract_actuator.hpp>
#include <utility.hpp>

#include <ESP32Servo.h>

namespace OH {
    class ServoActuator : public AbstractActuator {
      public:
        ServoActuator(const uint8_t pin, const uint16_t min = 500, const uint16_t max = 2400) :
          pin(pin), min(min), max(max){};

        void setup() override
        {
            servo.attach(this->pin, this->min, this->max);
            this->writeOutput(0);
        };

        void writeOutput(oh_output_intensity_t intensity) override
        {
            servo.writeMicroseconds(accurateMap<uint16_t>(intensity, 0, OH_OUTPUT_INTENSITY_MAX, this->min, this->max));
        };

      protected:
        Servo servo = Servo();
        uint8_t pin;
        uint16_t min, max;
    };
} // namespace OH
