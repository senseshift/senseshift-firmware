#include <Arduino.h>

#include <utility.hpp>
#include <output_writers/pwm.hpp>

namespace OH {
  uint8_t PWMOutputWriter::CHANNELS = 0;

  void PWMOutputWriter::setup() {
    this->chan = PWMOutputWriter::CHANNELS++;

  #if defined(ESP32)
    ledcSetup(this->chan, this->freq, this->resolution);
    ledcAttachPin(this->pin, this->chan);
  #else
    pinMode(this->pin, OUTPUT);
  #endif
  };

  void PWMOutputWriter::writeOutput(oh_output_intensity_t intensity) {
  #if defined(ESP32)
    ledcWrite(chan, simpleMap<uint16_t>(intensity, OH_OUTPUT_INTENSITY_MAX, 4096));
  #else
    analogWrite(this->pin, simpleMap<uint16_t>(intensity, OH_OUTPUT_INTENSITY_MAX, 255));
  #endif
  };
}; // namespace OH
