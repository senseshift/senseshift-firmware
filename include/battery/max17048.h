#pragma once

#include <abstract_battery.hpp>
#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>


class MAX1704_BatteryLevel : public OH::Core::AbstractBattery
{
    private:
        bool active = false;
        SFE_MAX1704X* gauge;
    protected:
        uint8_t updateLevel() override;
    public:
        MAX1704_BatteryLevel(SFE_MAX1704X* gauge) : gauge(gauge) {};
        void setup() override;
};
