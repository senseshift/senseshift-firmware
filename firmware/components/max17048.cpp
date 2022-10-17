#include "openhaptics.h"
#include "components/max17048.h"

// @see https://github.com/sparkfun/SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library/blob/main/examples/Example1_Simple/Example1_Simple.ino

void MAX1704_BatteryLevel::setup()
{
    // Set up the MAX17043 LiPo fuel gauge:
    if (this->gauge->begin() == false) // Connect to the MAX17043 using the default wire port
    {
        // Serial.println(F("MAX17043 not detected. Please check wiring. Freezing."));
        while (1);
    }

    // Quick start restarts the MAX17043 in hopes of getting a more accurate
	// guess for the SOC.
	this->gauge->quickStart();

	// We can set an interrupt to alert when the battery SoC gets too low.
	// We can alert at anywhere between 1% - 32%:
	this->gauge->setThreshold(20); // Set alert threshold to 20%.
}