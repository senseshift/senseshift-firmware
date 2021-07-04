#include <BLEDevice.h>

#include "connections/ble.h"

void BLEConnection::setup()
{
    uint8_t newMac[8] = BLUETOOTH_ADDRESS;
    esp_base_mac_addr_set(newMac);

    BLEDevice::init(this->deviceName);

    this->bleServer = BLEDevice::createServer();
}

void BLEConnection::loop()
{
    
}