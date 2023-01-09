#pragma once

#include <abstract_battery.hpp>

#include <Adafruit_INA219.h>

class INA219_BatteryLevel : public OH::AbstractBattery
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
