#pragma once

#include <abstract_battery.hpp>

#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>

namespace OH {
  class MAX1704_Battery : public OH::IBatterySensor {
   private:
    bool active = false;
    SFE_MAX1704X* gauge;

   protected:

   public:
    MAX1704_Battery(SFE_MAX1704X* gauge) : gauge(gauge) {};

    void setup() override;
    BatteryState getValue() override;
  };
}
