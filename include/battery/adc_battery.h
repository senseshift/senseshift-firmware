#pragma once

#include <abstract_battery.hpp>

class ADCBattery : public OH::Core::AbstractBattery {
 private:
  uint8_t pin;

 protected:
  uint8_t updateLevel() override;

 public:
  ADCBattery(const uint8_t pin) : pin(pin){};
};
