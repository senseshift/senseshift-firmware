#pragma once

#include <abstract_battery.hpp>

namespace OH {
  class ADCNaiveBattery : public OH::AbstractBattery {
   private:
    uint8_t pin;

   protected:
    uint8_t updateLevel() override;

   public:
    ADCNaiveBattery(const uint8_t pin, OH::BatteryConfig config, OH::IEventDispatcher* eventDispatcher, TaskConfig taskConfig = { "ADC Battery", 2048, BATTERY_TASK_PRIORITY, tskNO_AFFINITY })
      : AbstractBattery(config, eventDispatcher, taskConfig), pin(pin) {};
    ADCNaiveBattery(const uint8_t pin, OH::BatteryConfig config, OH::IEventDispatcher* eventDispatcher, const BaseType_t coreId = tskNO_AFFINITY)
      : AbstractBattery(config, eventDispatcher, { "ADC Battery", 2048, BATTERY_TASK_PRIORITY, coreId }), pin(pin) {};

    void setup() override;
  };
}
