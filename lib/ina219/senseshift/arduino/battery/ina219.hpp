#pragma once

#include <senseshift/battery/sensor.hpp>

#include <Adafruit_INA219.h>

namespace SenseShift::Arduino::Battery {
    class INA219_Battery : public ::SenseShift::Battery::IBatterySensor {
      private:
        bool active = false;
        Adafruit_INA219* sensor;

      public:
        INA219_Battery(Adafruit_INA219* sensor) : sensor(sensor){};

        void setup() override { this->active = this->sensor->begin(); }
        ::SenseShift::Battery::BatteryState getValue() override
        {
            if (!this->active) {
                return { 0 };
            }

            auto batteryVoltage = this->sensor->getBusVoltage_V();
            // TODO: change this linear transformation to smth more useful
            auto batteryPercentage = (batteryVoltage - 3.0) / 0.96;

            return {
                simpleMap<float>(batteryPercentage, 1.0f, 255.0f),
            };
        }
    };
} // namespace SenseShift::Arduino::Battery
