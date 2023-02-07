#pragma once

#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>
#include <abstract_battery.hpp>

class MAX1704_BatteryLevel : public OH::AbstractBattery {
 private:
  bool active = false;
  SFE_MAX1704X* gauge;

 protected:
  uint8_t updateLevel() override;

 public:
  MAX1704_BatteryLevel(SFE_MAX1704X* gauge, OH::IEventDispatcher* eventDispatcher) : AbstractBattery({ .sampleRate = BATTERY_SAMPLE_RATE }, eventDispatcher), gauge(gauge){};
  void setup() override;
};
