#pragma once

#include <cstdint>

#include "senseshift/battery/battery.hpp"

#include <senseshift/core/helpers.hpp>
#include <senseshift/input/sensor.hpp>

namespace SenseShift::Battery::Input {
/// Abstract battery sensor
using IBatterySensor = ::SenseShift::Input::Sensor<BatteryState>;

/// Interpolate voltage according to a lookup table.
template<typename Container>
class LookupTableInterpolateBatterySensor : public IBatterySensor {
    static_assert(std::is_same_v<typename Container::key_type, typename BatteryState::VoltageType>);
    static_assert(std::is_same_v<typename Container::mapped_type, float>);

  public:
    using VoltageType = typename BatteryState::VoltageType;
    using VoltageSource = ::SenseShift::Input::Sensor<VoltageType>;

    LookupTableInterpolateBatterySensor(VoltageSource* voltage_source, Container* lookup_table) :
      IBatterySensor(), voltage_source_(voltage_source), lookup_table_(lookup_table)
    {
    }

    void init() override
    {
        this->voltage_source_->init();
        this->voltage_source_->addValueCallback([this](VoltageType voltage) {
            // Current level in % (0.0 - 1.0)
            auto level = this->lookupInterpolateLevel(voltage);

            LOG_D("battery.sensor", "voltage=%f, level=%f", voltage, level);

            const BatteryState value = {
                .level = static_cast<typename BatteryState::LevelType>(level * BatteryState::MAX_LEVEL),
            };

            this->publishState(value);
        });
    }

  protected:
    auto lookupInterpolateLevel(VoltageType voltage) -> float
    {
        return ::SenseShift::lookup_table_interpolate_linear<Container, VoltageType, float>(
          *this->lookup_table_,
          voltage
        );
    }

  private:
    VoltageSource* voltage_source_;
    Container* lookup_table_;
};
} // namespace SenseShift::Battery::Input
