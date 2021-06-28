#pragma once

#include "connection.h"
#include "connections/ble.h"

class BHapticsBLEConnection final : public BLEConnection
{
    private:
        BLEService* motorService = nullptr;
    public:
        BHapticsBLEConnection(std::string deviceName) : BLEConnection(deviceName) {};
        void setup(void) override;
};
