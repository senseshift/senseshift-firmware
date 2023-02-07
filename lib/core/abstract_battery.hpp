#pragma once

#include "abstract_component.hpp"
#include "sensor.hpp"
#include "events.hpp"

#include <stdint.h>

#ifndef EVENT_BATTERY_LEVEL
#define EVENT_BATTERY_LEVEL "battery_level"
#endif

namespace OH {
  class BatteryLevelEvent : public IEvent
  {
   public:
    const uint8_t level;
    BatteryLevelEvent(const uint8_t level): IEvent(EVENT_BATTERY_LEVEL), level(level) {};
  };

  struct BatteryConfig {
    int sampleRate;
  };

  class AbstractBattery : public AbstractComponent, public ISensor<uint8_t> {
   protected:
    BatteryConfig config;
    IEventDispatcher* eventDispatcher;
    unsigned long last_battery_sample = 0;
    uint8_t level = 0;  // 0 = min, 255 = max
    virtual uint8_t updateLevel(void) = 0;

   public:
    AbstractBattery(BatteryConfig config, IEventDispatcher* eventDispatcher) : config(config), eventDispatcher(eventDispatcher) {};
    void loop(void) override;
    uint8_t getValue() override { return this->level; };
  };

  class IBatteryConnected {
   public:
    virtual AbstractBattery* getBattery() = 0;
  };
}  // namespace OH