#pragma once

#include "output.h"

class DIOOutputWriter : public OutputWriter {
 private:
  uint8_t pin, intensityThreshold;
  unsigned long highTime;
  unsigned long coolDownTime;
  uint8_t state;
  uint8_t desiredState;

 public:
  DIOOutputWriter(const uint8_t pin, const uint8_t intensityThreshold) : pin(pin), intensityThreshold(intensityThreshold){};

  void setup() override;
  void writeOutput(outputIntensity_t intensity) override;
  void loop() override;
};
