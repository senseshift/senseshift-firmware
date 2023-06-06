#pragma once

#include <sensor.hpp>
#include <calibration.hpp>

#include <Arduino.h>

namespace OpenGloves
{
  struct IFinger {
    //
  };

  class FingerSensor : public IFinger, public OH::CalibratedSensor<uint16_t>
  {
    private:
      uint8_t pin;
      bool invert;

    protected:
      uint16_t updateValue(void) override {
        auto value = analogRead(this->pin);

        if (this->invert) {
          value = ANALOG_MAX - value;
        }

        // TODO: add median filter

        return value;
      };

    public:
      FingerSensor(uint8_t pin, bool invert, OH::Calibrator<uint16_t>* calibrator) :
        OH::CalibratedSensor<uint16_t>(calibrator),
        pin(pin), invert(invert) {};

      void setup(void) {
        pinMode(this->pin, INPUT);
      };
  };

  // TODO: add splay finger sensor

} // namespace OpenGloves
