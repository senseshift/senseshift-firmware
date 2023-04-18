#include <Arduino.h>

#include <output_writers/pwm.hpp>

uint8_t PWMOutputWriter::CHANNELS = 0;

void PWMOutputWriter::setup() {
  this->chan = PWMOutputWriter::CHANNELS++;

#if defined(ARDUINO_ARCH_ESP32)
  ledcSetup(this->chan, this->freq, this->resolution);
  ledcAttachPin(this->pin, this->chan);
#else
  pinMode(this->pin, OUTPUT);
#endif
};

void PWMOutputWriter::writeOutput(oh_output_intensity_t intensity) {
#if defined(ARDUINO_ARCH_ESP32)
  ledcWrite(chan, (uint16_t)map(intensity, 0, OH_OUTPUT_INTENSITY_MAX, 0, 4096));
#else
  analogWrite(this->pin, (uint16_t)map(intensity, 0, OH_OUTPUT_INTENSITY_MAX, 0, 255));
#endif
}
