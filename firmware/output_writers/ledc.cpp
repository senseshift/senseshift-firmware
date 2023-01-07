#include <Arduino.h>

#include "config/all.h"
#include "output_writers/ledc.h"

uint8_t LEDCOutputWriter::CHANNELS = 0;

void LEDCOutputWriter::setup()
{
    this->chan = LEDCOutputWriter::CHANNELS++;

    ledcSetup(this->chan, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(this->pin, this->chan);

    // Serial.printf("> %s: attached channel %2u to pin %2u\n", __PRETTY_FUNCTION__, this->chan, this->pin);
};

void LEDCOutputWriter::writeOutput(outputIntensity_t intensity)
{
    ledcWrite(chan, (uint16_t)map(intensity, 0, UINT16_MAX, 0, 4096));
}
