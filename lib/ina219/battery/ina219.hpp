#pragma once

#include <abstract_battery.hpp>

#include <Adafruit_INA219.h>

namespace OH {
  class INA219_Battery : public OH::AbstractBattery {
   private:
    bool active = false;
    Adafruit_INA219* sensor;

   protected:
    uint8_t updateLevel() override;

   public:
    INA219_Battery(Adafruit_INA219* sensor, OH::BatteryConfig config, OH::IEventDispatcher* eventDispatcher, const BaseType_t coreId = tskNO_AFFINITY)
      : AbstractBattery(config, eventDispatcher, { "INA219 Battery", 1024, BATTERY_TASK_PRIORITY, coreId }),
      sensor(sensor) {};
    INA219_Battery(Adafruit_INA219* sensor, OH::BatteryConfig config, OH::IEventDispatcher* eventDispatcher, TaskConfig taskConfig = { "INA219 Battery", 1024, BATTERY_TASK_PRIORITY, tskNO_AFFINITY })
      : AbstractBattery(config, eventDispatcher, taskConfig),
      sensor(sensor) {};

    void setup() override;
  };
}
