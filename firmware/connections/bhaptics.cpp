#include "config/bhaptics.h"

#include "connections/bhaptics.h"
#include "output.h"

class MotorCharCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic) override
    {
        auto value = pCharacteristic->getValue();
        uint8_t* data = (uint8_t*) value.data();
    };
};

void BHapticsBLEConnection::setup()
{
    BLEConnection::setup();

    this->motorService = this->bleServer->createService(BH_BLE_SERVICE_MOTOR_UUID);

    auto* motorChar = this->motorService->createCharacteristic(BH_BLE_SERVICE_MOTOR_CHAR_MOTOR_UUID, BLECharacteristic::PROPERTY_WRITE_NR);
    motorChar->setCallbacks(new MotorCharCallbacks());

    this->motorService->start();

    auto advertising = this->bleServer->getAdvertising();
    advertising->addServiceUUID(BH_BLE_SERVICE_MOTOR_UUID);
    advertising->start();
}
