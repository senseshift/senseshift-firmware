#pragma once

#include <senseshift/output/actuator.hpp>
#include <senseshift/utility.hpp>

#include <Arduino.h>

namespace SenseShift::Arduino::Output {
    class ActuatorPWM : public ::SenseShift::Output::IActuator<std::uint16_t> {
      public:
        static inline constexpr const std::uint16_t MAX_INTENSITY = 4095;

        ActuatorPWM(const std::uint8_t pin, const double freq = 60, const std::uint8_t resolution = 12) :
          pin(pin), freq(freq), resolution(resolution){};

        void setup() override
        {
            this->chan = CHANNELS++;

#if defined(ESP32)
            ledcSetup(this->chan, this->freq, this->resolution);
            ledcAttachPin(this->pin, this->chan);
#else
            pinMode(this->pin, OUTPUT);
#endif
        }

        void writeOutput(std::uint16_t intensity) override
        {
#if defined(ESP32)
            ledcWrite(
              chan,
              ::SenseShift::simpleMap<std::uint16_t>(intensity, MAX_INTENSITY, (1 << this->resolution) - 1)
            );
#else
            // Arduino only supports 8-bit PWM
            analogWrite(this->pin, ::SenseShift::simpleMap<std::uint16_t>(intensity, MAX_INTENSITY, (1 << 8) - 1));
#endif
        }

      private:
        static inline std::uint8_t CHANNELS = 0;
        std::uint8_t pin, chan;
        double freq;
        std::uint8_t resolution;
    };
}; // namespace SenseShift::Arduino::Output
