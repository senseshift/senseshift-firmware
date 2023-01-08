#pragma once

#include "battery/abstract_battery.h"

class ADCBattery : public AbstractBattery {
 private:
  uint8_t pin;

 protected:
  uint8_t updateLevel() override;

 public:
  ADCBattery(const uint8_t pin) : pin(pin){};
};
