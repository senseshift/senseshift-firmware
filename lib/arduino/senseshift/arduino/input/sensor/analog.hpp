#pragma once

#include <cstdint>

#include <senseshift/input/sensor/sensor.hpp>

#include <Arduino.h>

#if defined(__AVR__)
#define ANALOG_MAX 1023.0F
#elif defined(ESP32)
#define ANALOG_MAX 4095.0F
#elif !defined(ANALOG_MAX)
#warning "This board doesn't have an auto ANALOG_MAX assignment, please set it manually"
#define ANALOG_MAX static_assert(false, "ANALOG_MAX is not defined")
// Uncomment and set as needed (only touch if you know what you are doing)
// #define ANALOG_MAX 4095.0F
#endif

namespace SenseShift::Arduino::Input {
    class AnalogSimpleSensor : public ::SenseShift::Input::IFloatSimpleSensor {
        const std::uint8_t pin_;

      public:
        static constexpr float MAX_VALUE = ANALOG_MAX;

        explicit AnalogSimpleSensor(const std::uint8_t pin) : pin_(pin) {}

        void init() override { pinMode(this->pin_, INPUT); };

        [[nodiscard]] inline auto getValue() -> float override
        {
            const std::uint16_t raw = analogRead(this->pin_);
            return static_cast<float>(raw) / ANALOG_MAX;
        }
    };
} // namespace SenseShift::Arduino::Input
