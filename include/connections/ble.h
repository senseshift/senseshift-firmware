#pragma once

#include <string>

#include <BLEServer.h>

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
