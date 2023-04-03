#include <Arduino.h>

#include "config/all.h"
#include "output_writers/dio.h"

#define SOL_ON 0x1
#define SOL_OFF 0x0
#define SOL_COOLDOWN 0x2

void DIOOutputWriter::setup() {
  pinMode(this->pin, OUTPUT);

  this->highTime = 0;
  this->coolDownTime = 0;
  this->state = SOL_OFF;
  this->desiredState = SOL_OFF;

  digitalWrite(pin, LOW);

  // Serial.printf("> %s: attached to pin %2u\n",
  // __PRETTY_FUNCTION__, this->pin);
}

void DIOOutputWriter::writeOutput(outputIntensity_t intensity) {
  if (intensity > this->intensityThreshold) {
    this->desiredState = SOL_ON;
  }
}

void DIOOutputWriter::loop() {
  unsigned long now = millis();

  switch(this->state) {
    case SOL_ON:
      if (now - this->highTime > 25) {
        digitalWrite(pin, LOW);
        this->coolDownTime = now;
        this->state = SOL_COOLDOWN;
      }
      break;
    case SOL_COOLDOWN:
      if (now - this->coolDownTime > 25) {
        this->state = SOL_OFF;
      }
      break;
    case SOL_OFF:
      if (desiredState == SOL_ON) {
        this->state = SOL_ON;
        digitalWrite(pin, HIGH);
        this->highTime = now;
        this->desiredState = SOL_OFF;      }
  }
}
