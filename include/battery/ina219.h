#pragma once

#include <Adafruit_INA219.h>

#include "battery/abstract_battery.h"

class INA219_BatteryLevel : public AbstractBattery
{
    private:
        bool active = false;
        Adafruit_INA219* sensor;
    protected:
        uint8_t updateLevel() override;
    public:
        INA219_BatteryLevel(Adafruit_INA219* sensor) : sensor(sensor) {};
        void setup() override;
};
