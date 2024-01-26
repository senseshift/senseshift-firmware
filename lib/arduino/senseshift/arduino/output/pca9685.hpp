#pragma once

#include <cstdint>

#include <senseshift/output/output.hpp>
#include <senseshift/utility.hpp>

#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include <Wire.h>

namespace SenseShift::Arduino::Output {
    class PCA9685Output : public ::SenseShift::Output::FloatOutput {
      public:
        static inline constexpr std::uint16_t MAX_INTENSITY = 4095;

        PCA9685Output(Adafruit_PWMServoDriver* driver, const std::uint8_t num) : driver_(driver), channel_(num){};

        void init() override {
            this->driver_->begin();
        }

        void writeState(const ValueType intensity) override
        {
            this->driver_->setPin(
                this->channel_,
                intensity * MAX_INTENSITY
            );
        }

      private:
        Adafruit_PWMServoDriver* driver_;
        std::uint8_t channel_;
    };
} // namespace SenseShift::Arduino::Output
