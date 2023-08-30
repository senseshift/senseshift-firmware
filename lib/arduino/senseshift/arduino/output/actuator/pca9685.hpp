#pragma once

#include <senseshift/output/actuator.hpp>
#include <senseshift/utility.hpp>

#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include <Wire.h>

namespace SenseShift::Arduino::Output {
    class ActuatorPCA9685 : public ::SenseShift::Output::IActuator<std::uint16_t> {
      public:
        static inline constexpr const std::uint16_t MAX_INTENSITY = 4095;

        ActuatorPCA9685(Adafruit_PWMServoDriver* driver, const std::uint8_t num) : driver(driver), num(num){};

        void writeOutput(std::uint16_t intensity) override
        {
            this->driver->setPin(this->num, ::SenseShift::simpleMap<std::uint16_t>(intensity, MAX_INTENSITY, 4095));
        }

      private:
        Adafruit_PWMServoDriver* driver;
        std::uint8_t num;
    };
} // namespace SenseShift::Arduino::Output
