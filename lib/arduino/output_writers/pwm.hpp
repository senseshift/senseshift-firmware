#pragma once

#include <abstract_actuator.hpp>

namespace OH {
    class PWMOutputWriter : public OH::AbstractActuator {
      private:
        static uint8_t CHANNELS;
        uint8_t pin, chan;
        double freq;
        uint8_t resolution;

      public:
        PWMOutputWriter(const uint8_t pin, const double freq = 60, const uint8_t resolution = 12) :
          pin(pin), freq(freq), resolution(resolution){};

        void setup() override;
        void writeOutput(oh_output_intensity_t intensity) override;
    };
}; // namespace OH
