#include <BLEDevice.h>

#include "connections/ble.h"

void BLEConnection::setup()
{
    BLEDevice::init(this->deviceName);

    this->bleServer = BLEDevice::createServer();
}

void BLEConnection::loop()
{
    
}