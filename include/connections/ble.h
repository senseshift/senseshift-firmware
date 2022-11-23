#pragma once

#include "config/bluetooth.h"

#if defined(BLUETOOTH_USE_NIMBLE) && BLUETOOTH_USE_NIMBLE == true
    #include <NimBLEDevice.h>
#else
    #include <BLEDevice.h>
#endif

#include <string>

#include "connection.h"

class BLEConnection : public Connection
{
    protected:
        std::string deviceName;
        BLEServer* bleServer = nullptr;
    public:
        BLEConnection(const std::string &deviceName) : deviceName(deviceName) {};
        void setup(void) override;
        void loop(void) override;
};
