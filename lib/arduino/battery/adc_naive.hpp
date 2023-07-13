#pragma once

#include <abstract_battery.hpp>

namespace OH {
    class ADCNaiveBattery : public OH::IBatterySensor {
      private:
        uint8_t pin;

      public:
        ADCNaiveBattery(const uint8_t pin) : pin(pin){};

        BatteryState getValue() override;
        void setup();
    };
} // namespace OH
