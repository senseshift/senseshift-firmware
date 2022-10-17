#pragma once

#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>

#include "component.h"

class MAX1704_BatteryLevel : public Component
{
    protected:
        SFE_MAX1704X* gauge;
    public:
        MAX1704_BatteryLevel(SFE_MAX1704X &gauge) : gauge(&gauge) {};
        void setup(void) override;
        void loop(void) override;
};