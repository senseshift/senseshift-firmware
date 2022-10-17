#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "output.h"

class LEDCOutputWriter : public OutputWriter
{
    private:
        uint8_t chan;
    public:
        LEDCOutputWriter(const uint8_t chan): chan(chan) {};

        void writeOutput(outputIntensity_t intensity) override;
};
