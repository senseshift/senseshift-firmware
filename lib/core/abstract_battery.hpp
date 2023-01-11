#pragma once

#include "abstract_component.hpp"
#include "sensor.hpp"

#include <stdint.h>

namespace OH {
  struct BatteryConfig {
    int sampleRate;
  };

  class AbstractBattery : public AbstractComponent, public ISensor<uint8_t> {
   protected:
    BatteryConfig config;
    unsigned long last_battery_sample = 0;
    uint8_t level = 0;  // 0 = min, 255 = max
    virtual uint8_t updateLevel(void) = 0;

   public:
    AbstractBattery(BatteryConfig config) : config(config) {};
    void loop(void) override;
    uint8_t getValue() override { return this->level; };
  };

  class IBatteryConnected {
   public:
    virtual AbstractBattery* getBattery() = 0;
  };
}  // namespace OH
