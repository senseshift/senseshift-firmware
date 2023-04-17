#pragma once

#include "sensor.hpp"
#include "events.hpp"

#include <stdint.h>

#ifndef BATTERY_TASK_PRIORITY
#define BATTERY_TASK_PRIORITY 1
#endif

namespace OH {
  class BatteryLevelEvent : public IEvent
  {
   public:
    const uint8_t level;
    BatteryLevelEvent(const uint8_t level): IEvent(OH_EVENT_BATTERY_LEVEL), level(level) {};
  };

  struct BatteryConfig {
    uint sampleRate;
  };

  class AbstractBattery : public ThrottledSensor<uint8_t> {
   friend class Task<ThrottledSensor<uint8_t>>;
   friend class ThrottledSensor<uint8_t>;

   private:
    void run(void) override;
   protected:
    IEventDispatcher* eventDispatcher;

    virtual void setup(void) {};
    uint8_t updateValue() override;

   public:
    AbstractBattery(BatteryConfig config, IEventDispatcher* eventDispatcher, TaskConfig taskConfig) : ThrottledSensor<uint8_t>(taskConfig, config.sampleRate), eventDispatcher(eventDispatcher) {};
    void begin() override {
      this->setup();
      ThrottledSensor<uint8_t>::begin();
    };
  };

  /**
   * Interface for components, that are connected to a battery
   */
  class IBatteryConnected {
   public:
    virtual AbstractBattery* getBattery() = 0;
  };
}  // namespace OH
