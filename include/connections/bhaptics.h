#pragma once

#include "connection.h"
#include "connections/ble.h"

class BHapticsBLEConnection final : public BLEConnection
{
    private:
        BLEService* motorService = nullptr;

        #if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
            unsigned long lastBatteryUpdate = 0;
            BLECharacteristic* batteryChar = nullptr;
        #endif
    protected:
        void (*motorTransformer)(std::string&);
    public:
        BHapticsBLEConnection(std::string deviceName, void (*motorTransformer)(std::string&)) : BLEConnection(deviceName), motorTransformer(motorTransformer) {};
        void setup(void) override;
};
