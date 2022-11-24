#pragma once

#include "output.h"

class LEDCOutputWriter : public OutputWriter
{
    private:
        static uint8_t CHANNELS;
        uint8_t pin, chan;
    public:
        LEDCOutputWriter(const uint8_t pin): pin(pin) {};

        void setup() override;
        void writeOutput(outputIntensity_t intensity) override;
};
