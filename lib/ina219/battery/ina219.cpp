#include "battery/ina219.hpp"

namespace OH {
  void OH::INA219_Battery::setup() {
    this->active = this->sensor->begin();
  }

  BatteryState OH::INA219_Battery::getValue() {
    if (!this->active) {
      return { 0 };
    }

    auto batteryVoltage = this->sensor->getBusVoltage_V();
    // TODO: change this linear transformation to smth more useful
    auto batteryPercentage = (batteryVoltage - 3.0) / 0.96;

    return {
      map(batteryPercentage, 0.0f, 1.0f, 0, 255)
    };
  }
}
