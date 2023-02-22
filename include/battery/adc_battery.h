#pragma once

#include "config/all.h"

#include <abstract_battery.hpp>

class ADCBattery : public OH::AbstractBattery {
 private:
  uint8_t pin;

 protected:
  uint8_t updateLevel() override;

 public:
  ADCBattery(const uint8_t pin, OH::BatteryConfig config, OH::IEventDispatcher* eventDispatcher, const BaseType_t coreId)
    : AbstractBattery(config, eventDispatcher, { "ADC Battery", 4096, BATTERY_TASK_PRIORITY, coreId }),
    pin(pin) {};
  ADCBattery(const uint8_t pin, OH::BatteryConfig config, OH::IEventDispatcher* eventDispatcher)
    : AbstractBattery(config, eventDispatcher, { "ADC Battery", 4096, BATTERY_TASK_PRIORITY, tskNO_AFFINITY }),
    pin(pin) {};

  void setup() override;
};
