#pragma once

#include <cstdint>

#include <Arduino.h>

#include <senseshift/output/output.hpp>

namespace SenseShift::Arduino::Output {
    /// Arduino analog output
    class AnalogOutput : public ::SenseShift::Output::FloatOutput {
      public:
        static inline constexpr std::uint16_t MAX_INTENSITY = 255;

        explicit AnalogOutput(const std::uint8_t pin) : pin_(pin) {
        }

        void init() override {
            pinMode(this->pin_, OUTPUT);
        }

        void writeState(const float value) override {
            analogWrite(this->pin_, value * MAX_INTENSITY);
        }

      private:
        uint8_t pin_;
    };
} // namespace SenseShift::Arduino::Output
