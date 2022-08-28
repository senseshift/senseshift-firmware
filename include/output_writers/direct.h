#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "output.h"

class DirectOutputWriter : public OutputWriter
{
    private:
        uint8_t chan;
    public:
        DirectOutputWriter(const uint8_t chan): chan(chan) {};

        void writeOutput(outputIntensity_t intensity) override;
};
