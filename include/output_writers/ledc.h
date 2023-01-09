#pragma once

#include "output.h"
#include <abstract_output_writer.hpp>

class LEDCOutputWriter : public OH::AbstractOutputWriter {
 private:
  static uint8_t CHANNELS;
  uint8_t pin, chan;

 public:
  LEDCOutputWriter(const uint8_t pin) : pin(pin){};

  void setup() override;
  void writeOutput(oh_outputIntensity_t intensity) override;
};
