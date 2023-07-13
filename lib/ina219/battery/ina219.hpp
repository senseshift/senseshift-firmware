#pragma once

#include <abstract_battery.hpp>

#include <Adafruit_INA219.h>

namespace OH {
    class INA219_Battery : public OH::IBatterySensor {
      private:
        bool active = false;
        Adafruit_INA219* sensor;

      public:
        INA219_Battery(Adafruit_INA219* sensor) : sensor(sensor){};

        void setup() override;
        BatteryState getValue() override;
    };
} // namespace OH
