#include "config/battery.h"

#include "battery/ina219.h"
#include "openhaptics.h"

void INA219_BatteryLevel::setup() {
  this->active = this->sensor->begin();
}

uint8_t INA219_BatteryLevel::updateLevel() {
  if (!this->active) {
    return 0;
  }

  auto batteryVoltage = this->sensor->getBusVoltage_V();
  // TODO: change this linear transformation to smth more useful
  auto batteryPercentage = (batteryVoltage - 3.0) / 0.96;

  return map(batteryPercentage, 0.0f, 1.0f, 0, 255);
}
