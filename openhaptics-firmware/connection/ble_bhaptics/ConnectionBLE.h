#ifndef CONNECTION_BLE_H
#define CONNECTION_BLE_H

#include "../Connection.h"

#include "service/BHService.h"
#include "service/MotorService.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

class ConnectionBLE : public Connection
{
    private:
        bool started = false;
        std::string deviceName;

        BLEServer* bleServer;

        MotorService* motorService;
    protected:
        MotorService* setupMotorService(BLEServer*);
    public:
        ConnectionBLE(std::string);
        ~ConnectionBLE();
        void start(void);
        void init(void);
        void loop(void);
        bool isOpen(void);
};

#endif /* CONNECTION_BLE_H */