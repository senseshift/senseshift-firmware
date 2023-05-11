#pragma once

#include <sensor.hpp>

#include <Arduino.h>

namespace OH {
  class JoystickAxisSensor : public ISensor<uint16_t> {
    private:
      uint8_t pin;
      float dead_zone;
      bool invert;

      int filterDeadZone(int in) {
        // This function clamps the input to the center of the range if
        // the value is within the threshold. This is to eliminate at-rest
        // noise of the joystick.
        int center = ANALOG_MAX / 2;
        return abs(center - in) < dead_zone * ANALOG_MAX ? center : in;
      }

    protected:
      uint16_t getValue(void) override {
        auto value = analogRead(this->pin);
        value = this->filterDeadZone(value);

        if (this->invert) {
          value = ANALOG_MAX - value;
        }

        return value;
      }

    public:
      JoystickAxisSensor(uint8_t pin, float dead_zone, bool invert = false) : pin(pin), dead_zone(dead_zone), invert(invert) {}
  };
}; // namespace OH
