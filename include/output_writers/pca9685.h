#pragma once

#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include <Wire.h>

#include "output.h"

class PCA9685OutputWriter : public OutputWriter {
 private:
  Adafruit_PWMServoDriver* driver;
  uint8_t num;

 public:
  PCA9685OutputWriter(Adafruit_PWMServoDriver* driver, const uint8_t num)
      : driver(driver), num(num){};

  void writeOutput(outputIntensity_t intensity) override;
};
