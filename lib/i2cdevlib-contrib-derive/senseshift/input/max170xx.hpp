#pragma once

#include <senseshift/battery/input/battery_sensor.hpp>

#include <i2cdev/max170xx.hpp>

namespace SenseShift::Battery::Input {
/// MAX170xx battery sensor
template<typename MAX170XX>
class MAX170XXBatterySensor : public ::SenseShift::Input::ISimpleSensor<BatteryState> {
  public:
    MAX170XXBatterySensor(MAX170XX& driver) : ISimpleSensor(), driver_(driver)
    {
    }

    void init() override
    {
        if (this->driver_.check() != I2CDEV_RESULT_OK) {
            LOG_E("battery.sensor", "Failed to initialize MAX170xx");
            return;
        }
    }

    BatteryState getValue() override
    {
        return {
            .level = static_cast<BatteryState::LevelType>(this->driver_.readSoc() * BatteryState::MAX_LEVEL),
        };
    }

  private:
    MAX170XX& driver_;
};
} // namespace SenseShift::Battery::Input
