#include "connections/bhaptics.h"

#include "config/bhaptics.h"

void BHapticsBLEConnection::setup()
{
    BLEConnection::setup();

    this->motorService = this->bleServer->createService(BH_BLE_SERVICE_MOTOR_UUID);
    
    this->motorService->start();
    this->bleServer->getAdvertising()->start();
}
