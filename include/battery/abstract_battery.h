#pragma once

#include <stdint.h>
#include "component.h"
#include "../config/battery.h"

class AbstractBattery : Component
{
    protected:
        unsigned long last_battery_sample = 0;
        uint8_t level = 0; // 0 = min, 255 = max
        virtual uint8_t updateLevel(void) = 0;
    public:
        void setup(void) override;
        void loop(void) override;
        uint8_t getLevel() { return this->level; };
};
