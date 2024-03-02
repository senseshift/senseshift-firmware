#pragma once

#include <cstdint>

#include <senseshift/core/helpers.hpp>
#include <senseshift/output/output.hpp>

#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include <Wire.h>

namespace SenseShift::Arduino::Output {
    class PCA9685Output : public ::SenseShift::Output::IFloatOutput {
      public:
        static inline constexpr std::uint16_t MAX_INTENSITY = 4095;

        PCA9685Output(Adafruit_PWMServoDriver* driver, const std::uint8_t channel) :
          driver_(driver), channel_(channel){};

        void init() override { this->driver_->begin(); }

        void writeState(const ValueType value) override
        {
            const auto duty = static_cast<std::uint16_t>(value * MAX_INTENSITY);
            this->driver_->setPin(this->channel_, duty);
        }

      private:
        Adafruit_PWMServoDriver* driver_;
        std::uint8_t channel_;
    };

    class PCA9685ServoOutput : public ::SenseShift::Output::IFloatOutput {
      public:
        PCA9685ServoOutput(
          Adafruit_PWMServoDriver* const driver,
          const std::uint8_t channel,
          const std::uint16_t min_pulse = 544,
          const std::uint16_t max_pulse = 2400
        ) :
          driver_(driver), channel_(channel), min_pulse_(min_pulse), max_pulse_(max_pulse)
        {
        }

        void init() override
        {
            this->driver_->begin();
            this->driver_->setPWMFreq(50);
        }

        void writeState(const ValueType value) override
        {
            const auto angle = value * 180;
            const auto duty = ::SenseShift::remap<uint16_t, float>(angle, 0, 180, this->min_pulse_, this->max_pulse_);

            this->driver_->writeMicroseconds(this->channel_, duty);
        }

      private:
        Adafruit_PWMServoDriver* const driver_;
        std::uint8_t channel_;
        std::uint16_t min_pulse_;
        std::uint16_t max_pulse_;
    };
} // namespace SenseShift::Arduino::Output
