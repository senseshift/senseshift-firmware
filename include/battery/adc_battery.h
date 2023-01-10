#pragma once

#include "config/all.h"

#include <abstract_battery.hpp>

class ADCBattery : public OH::AbstractBattery {
 private:
  uint8_t pin;

 protected:
  uint8_t updateLevel() override;

 public:
  ADCBattery(const uint8_t pin) : AbstractBattery({ .sampleRate = BATTERY_SAMPLE_RATE }), pin(pin) {};
};
