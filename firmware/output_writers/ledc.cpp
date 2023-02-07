#include <Arduino.h>

#include "config/all.h"
#include "output_writers/ledc.h"

uint8_t LEDCOutputWriter::CHANNELS = 0;

void LEDCOutputWriter::setup() {
  this->chan = LEDCOutputWriter::CHANNELS++;

  ledcSetup(this->chan, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(this->pin, this->chan);
};

void LEDCOutputWriter::writeOutput(oh_output_intensity_t intensity) {
  ledcWrite(chan, (uint16_t)map(intensity, 0, OH_OUTPUT_INTENSITY_MAX, 0, 4096));
}
