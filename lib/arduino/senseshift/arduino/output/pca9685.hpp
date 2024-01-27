#pragma once

#include <cstdint>

#include <senseshift/output/output.hpp>
#include <senseshift/utility.hpp>

#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include <Wire.h>

namespace SenseShift::Arduino::Output {
    class PCA9685Output : public ::SenseShift::Output::IFloatOutput {
      public:
        static inline constexpr std::uint16_t MAX_INTENSITY = 4095;

        PCA9685Output(Adafruit_PWMServoDriver* driver, const std::uint8_t num) : driver_(driver), channel_(num){};

        void init() override {
            this->driver_->begin();
        }

        void writeState(const ValueType value) override
        {
            const auto duty = static_cast<std::uint16_t>(value * MAX_INTENSITY);
            this->driver_->setPin(this->channel_, duty);
        }

      private:
        Adafruit_PWMServoDriver* driver_;
        std::uint8_t channel_;
    };
} // namespace SenseShift::Arduino::Output
