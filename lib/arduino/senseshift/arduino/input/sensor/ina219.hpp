#pragma once

#include <cstdint>

#include <senseshift/input/sensor.hpp>

#include <Adafruit_INA219.h>

namespace SenseShift::Arduino::Input {
    /// INA219 Current sensor
    /// TODO: refactor to component, that updates the value in multiple sensors (ESPHome style)
    class INA219CurrentSimpleSensor : public ::SenseShift::Input::IFloatSimpleSensor {
        Adafruit_INA219 ina219_;

      public:
        void init() override
        {
            if (!ina219_.success()) {
                log_e("Failed to find INA219 sensor");
            }
        }

        [[nodiscard]] inline auto getValue() -> float override { return ina219_.getCurrent_mA(); }
    };
} // namespace SenseShift::Arduino::Input