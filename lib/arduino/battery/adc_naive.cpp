#include "battery/adc_naive.hpp"

#include <Arduino.h>
#include <utility.hpp>

namespace OH {
    void ADCNaiveBattery::setup()
    {
        pinMode(this->pin, INPUT);
    }

    BatteryState ADCNaiveBattery::getValue()
    {
        return { .level = simpleMap<uint16_t>(analogRead(this->pin), 4095, 255) };
    }
} // namespace OH
