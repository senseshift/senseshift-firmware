#pragma once

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "connection/Connection.h"

#include "service/BHService.h"
#include "service/MotorService.h"

class ConnectionBH : public Connection
{
    private:
        bool started = false;
        std::string deviceName;

        BLEServer* bleServer;

        MotorService* motorService;
    public:
        ConnectionBH(std::string);
        ~ConnectionBH();
        void start(void);
        void init(void);
        void loop(void);
        bool isOpen(void);
};
