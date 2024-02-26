#pragma once

#include <cstddef>
#include <cstdint>

#ifdef ESP32
#include <ESP32Servo.h>
#else
#include <Servo.h>
#endif

#include <senseshift/output/output.hpp>

namespace SenseShift::Arduino::Output {
    class ServoOutput : public ::SenseShift::Output::IFloatOutput {
      public:
        ServoOutput(size_t pin) : pin_(pin){};

        void init() override { this->servo_.attach(this->pin_); }

        void writeState(const ValueType value) override
        {
            const auto duty = static_cast<std::uint16_t>(value * 180);
            this->servo_.write(duty);
        }

      private:
        Servo servo_;
        size_t pin_;
    };
} // namespace SenseShift::Arduino::Output