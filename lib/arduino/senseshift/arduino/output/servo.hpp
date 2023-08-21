#pragma once

#include <senseshift/output/actuator.hpp>
#include <senseshift/utility.hpp>

#include <ESP32Servo.h>

namespace SenseShift::Arduino::Output {
    class ServoActuator : public ::SenseShift::Output::IActuator<std::uint16_t> {
      public:
        static inline constexpr const std::uint16_t MAX_INTENSITY = 4095;

        ServoActuator(const uint8_t pin, const std::uint16_t min = 500, const std::uint16_t max = 2400) :
          pin(pin), min(min), max(max){};

        void setup() override
        {
            servo.attach(this->pin, this->min, this->max);
            this->writeOutput(0);
        };

        void writeOutput(std::uint16_t intensity) override
        {
            servo.writeMicroseconds(
              ::SenseShift::accurateMap<std::uint16_t>(intensity, 0, MAX_INTENSITY, this->min, this->max)
            );
        };

      private:
        Servo servo = Servo();
        uint8_t pin;
        std::uint16_t min, max;
    };
} // namespace SenseShift::Arduino::Output
