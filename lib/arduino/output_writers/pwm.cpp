#include <Arduino.h>

#include <output_writers/pwm.hpp>

uint8_t PWMOutputWriter::CHANNELS = 0;

void PWMOutputWriter::setup() {
  this->chan = PWMOutputWriter::CHANNELS++;

  ledcSetup(this->chan, this->freq, this->resolution);
  ledcAttachPin(this->pin, this->chan);
};

void PWMOutputWriter::writeOutput(oh_output_intensity_t intensity) {
  ledcWrite(chan, (uint16_t)map(intensity, 0, OH_OUTPUT_INTENSITY_MAX, 0, 4096));
}
