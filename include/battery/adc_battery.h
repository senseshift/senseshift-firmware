#pragma once

#include "config/all.h"

#include <abstract_battery.hpp>

class ADCBattery : public OH::AbstractBattery {
 private:
  uint8_t pin;

 protected:
  uint8_t updateLevel() override;

 public:
  ADCBattery(const uint8_t pin, OH::BatteryConfig config, OH::IEventDispatcher* eventDispatcher) : AbstractBattery(config, eventDispatcher), pin(pin) {};
};
