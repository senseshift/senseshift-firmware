#pragma once

#include "config/all.h"

#include <abstract_battery.hpp>

#include <Adafruit_INA219.h>

class INA219_BatteryLevel : public OH::AbstractBattery {
 private:
  bool active = false;
  Adafruit_INA219* sensor;

 protected:
  uint8_t updateLevel() override;

 public:
  INA219_BatteryLevel(Adafruit_INA219* sensor, OH::BatteryConfig config, OH::IEventDispatcher* eventDispatcher, const BaseType_t coreId)
    : AbstractBattery(config, eventDispatcher, { "INA219 Battery", 1024, BATTERY_TASK_PRIORITY, coreId }),
    sensor(sensor) {};
  INA219_BatteryLevel(Adafruit_INA219* sensor, OH::BatteryConfig config, OH::IEventDispatcher* eventDispatcher)
    : AbstractBattery(config, eventDispatcher, { "INA219 Battery", 1024, BATTERY_TASK_PRIORITY, tskNO_AFFINITY }),
    sensor(sensor) {};

  void setup() override;
};
