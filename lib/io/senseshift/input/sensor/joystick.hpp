#pragma once

#include "senseshift/input/sensor.hpp"

namespace SenseShift::Input {
    /**
     * Joystick axis sensor decorator
     */
    template<typename Tp>
    class JoystickAxisSensor : public ISimpleSensor<Tp> {
    public:
        using ValueType = Tp;

      private:
        ISimpleSensor<ValueType>* sensor_;
        float dead_zone_;

        /// This function clamps the input to the center of the range if
        /// the value is within the threshold. This is to eliminate at-rest
        /// noise of the joystick.
        [[nodiscard]] auto filterDeadZone(const int value_in) const -> ValueType {
            constexpr ValueType center = ANALOG_MAX / 2;
            return abs(center - value_in) < dead_zone_ * ANALOG_MAX ? center : value_in;
        }

      public:
        JoystickAxisSensor(ISimpleSensor<Tp>* sensor, const float dead_zone) : sensor_(sensor), dead_zone_(dead_zone){};

        void init() override { this->sensor_->init(); };

        [[nodiscard]] auto getValue() -> uint16_t override
        {
            auto value = this->sensor_->getValue();
            value = this->filterDeadZone(value);
            return value;
        }
    };
}; // namespace SenseShift::Input
