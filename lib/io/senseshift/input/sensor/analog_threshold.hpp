#pragma once

#include "senseshift/input/sensor.hpp"
#include <type_traits>

namespace SenseShift::Input {
template<typename Tp = float>
class AnalogThresholdSensor : public BinarySensor {
  public:
    /// Analog threshold sensor with hysteresis.
    ///
    /// \param source The source sensor.
    /// \param threshold_upper Upper threshold, that needs to be crossed to transition from `low` to `high` states.
    /// \param threshold_lower Lower threshold, that needs to be crossed to transition from `high` to `low` states.
    explicit AnalogThresholdSensor(
      Sensor<Tp>* source, Tp threshold_upper, Tp threshold_lower, bool attach_callbacks = false
    ) :
      source_(source),
      threshold_lower_(threshold_lower),
      threshold_upper_(threshold_upper),
      attach_callbacks_(attach_callbacks)
    {
    }

    /// \param source The source sensor.
    /// \param threshold Threshold, that will be used for both upper and lower thresholds.
    template<typename U = Tp, std::enable_if_t<std::is_same_v<U, float>, int> = 0>
    explicit AnalogThresholdSensor(Sensor<Tp>* source, float threshold = 0.5f, bool attach_callbacks = false) :
      AnalogThresholdSensor(source, threshold, threshold, attach_callbacks)
    {
    }

    void init() override
    {
        SS_SUBSENSOR_INIT(this->source_, this->attach_callbacks_, [this](Tp /*value*/) {
            this->recalculateState();
        });
    }

    void tick()
    {
        if (this->attach_callbacks_) {
            LOG_E(
              "sensor.analog_threshold",
              "tick() called when attach_callbacks_ is true, infinite loop go wroom-wroom!"
            );
        }
        this->recalculateState();
    }

    void recalculateState()
    {
        const auto sensor_value = this->source_->getValue();
        this->publishState(sensor_value >= (this->getValue() ? this->threshold_lower_ : this->threshold_upper_));
    }

  private:
    Sensor<Tp>* source_;

    Tp threshold_lower_, threshold_upper_;

    bool attach_callbacks_ = false;
};
} // namespace SenseShift::Input
