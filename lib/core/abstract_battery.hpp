#pragma once

#include "abstract_component.hpp"
#include "sensor.hpp"

#include <stdint.h>

namespace OH {
  class AbstractBattery : public AbstractComponent, public ISensor<uint8_t> {
   protected:
    unsigned long last_battery_sample = 0;
    uint8_t level = 0;  // 0 = min, 255 = max
    virtual uint8_t updateLevel(void) = 0;

   public:
    void loop(void) override;
    uint8_t getValue() override { return this->level; };
  };
}  // namespace OH
