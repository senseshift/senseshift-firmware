#pragma once

#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include <Wire.h>

#include "output.h"
#include <abstract_output_writer.hpp>

class PCA9685OutputWriter : public OH::AbstractOutputWriter {
 private:
  Adafruit_PWMServoDriver* driver;
  uint8_t num;

 public:
  PCA9685OutputWriter(Adafruit_PWMServoDriver* driver, const uint8_t num)
      : driver(driver), num(num){};

  void writeOutput(oh_outputIntensity_t intensity) override;
};
