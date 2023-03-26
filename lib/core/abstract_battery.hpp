#pragma once

#include "abstract_component.hpp"
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
    int sampleRate;
  };

  class AbstractBattery : public Task<AbstractBattery>, public ISensor<uint8_t> {
   friend class Task<AbstractBattery>;

   private:
    void run(void);
   protected:
    IEventDispatcher* eventDispatcher;
    uint8_t level = 0;  // 0 = min, 255 = max

    virtual void setup(void) {};
    virtual uint8_t updateLevel(void) = 0;

   public:
    AbstractBattery(BatteryConfig config, IEventDispatcher* eventDispatcher, TaskConfig taskConfig) : Task<AbstractBattery>(taskConfig), config(config), eventDispatcher(eventDispatcher) {};
    uint8_t getValue() override { return this->level; };
    void begin() override {
      this->setup();
      OH::Task<AbstractBattery>::begin();
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
