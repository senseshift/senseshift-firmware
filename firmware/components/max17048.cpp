#include "openhaptics.h"
#include "components/max17048.h"
#include "config/battery.h"

// @see https://github.com/sparkfun/SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library/blob/main/examples/Example1_Simple/Example1_Simple.ino

void MAX1704_BatteryLevel::setup()
{
    // Set up the MAX17043 LiPo fuel gauge:
    this->active = this->gauge->begin();

    if (this->active) { 
        // Quick start restarts the MAX17043 in hopes of getting a more accurate
        // guess for the SOC.
        this->gauge->quickStart();

        // We can set an interrupt to alert when the battery SoC gets too low.
        // We can alert at anywhere between 1% - 32%:
        this->gauge->setThreshold(20); // Set alert threshold to 20%.
    }
}

void MAX1704_BatteryLevel::loop()
{
    if (!this->active) {
        return;
    }

    auto now_ms = millis();

    if (now_ms - this->last_battery_sample >= BATTERY_SAMPLE_RATE) {
        this->last_battery_sample = now_ms;

        this->voltage = this->gauge->getVoltage();
        this->level = this->gauge->getSOC();
    }
}