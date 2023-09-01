#pragma once

#include <senseshift/battery/sensor.hpp>

#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>

namespace SenseShift::Arduino::Battery {
    class MAX1704_Battery : public ::SenseShift::Battery::IBatterySensor {
      public:
        MAX1704_Battery(SFE_MAX1704X* gauge) : gauge(gauge){};

        /**
         * @see
         * https://github.com/sparkfun/SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library/blob/main/examples/Example1_Simple/Example1_Simple.ino
         */
        void init() override
        {
            // Set up the MAX17043 LiPo fuel gauge:
            this->active = this->gauge->begin();

            if (this->active) {
                // Quick start restarts the MAX17043 in hopes of getting a more accurate
                // guess for the SOC.
                this->gauge->quickStart();

                // We can set an interrupt to alert when the battery SoC gets too low.
                // We can alert at anywhere between 1% - 32%:
                // this->gauge->setThreshold(SENSESHIFT_BATTERY_THRESHOLD_PERCENTAGE);
            }
        }

        ::SenseShift::Battery::BatteryState getValue() override
        {
            if (!this->active) {
                return { 0 };
            }

            return { .level = simpleMap(this->gauge->getSOC(), 1.0f, 255.0f) };
        }

      private:
        bool active = false;
        SFE_MAX1704X* gauge;
    };
} // namespace SenseShift::Arduino::Battery
