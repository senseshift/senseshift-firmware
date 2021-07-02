#include <HardwareSerial.h>
#include <BLE2902.h>

#include "config/bhaptics.h"

#include "connections/bhaptics.h"
#include "output.h"

class SerialOutputCharCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic) override
    {
        Serial.printf(">> onWrite (UUID: %s) \n", pCharacteristic->getUUID().toString().c_str());
        Serial.printf("\t value: `%s`, len: %u \n", pCharacteristic->getValue().c_str(), pCharacteristic->getValue().length());
    };
};

class MotorCharCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic) override
    {
        auto value = pCharacteristic->getValue();
        const size_t actLength = value.length() * 2;
        uint8_t values[actLength];

        for (size_t i = 0; i < 20; i++) {
            uint8_t byte = value[i];
            uint actIndex = i*2;
            values[actIndex] = (byte >> 4) & 0xf;
            values[actIndex + 1] = (byte & 0xf);
        }

        for (size_t i = 0; i < actLength; i++) {
            Serial.printf("%2hhu ", values[i]);
        }

        Serial.println();
    };
};

void BHapticsBLEConnection::setup()
{
    BLEConnection::setup();
    auto advertising = this->bleServer->getAdvertising();

    auto advertisingData = new BLEAdvertisementData();
    advertisingData->setAppearance(BH_BLE_APPEARANCE);
    advertisingData->setName(BLUETOOTH_NAME);

    advertising->setAdvertisementData(*advertisingData);
    advertising->setScanResponseData(*advertisingData);

    this->motorService = this->bleServer->createService(BH_BLE_SERVICE_MOTOR_UUID);

    auto* motorChar = this->motorService->createCharacteristic(BH_BLE_SERVICE_MOTOR_CHAR_MOTOR_UUID, BLECharacteristic::PROPERTY_WRITE_NR);
    motorChar->setCallbacks(new MotorCharCallbacks());

    auto* motorCharStable = this->motorService->createCharacteristic(BH_BLE_SERVICE_MOTOR_CHAR_MOTOR_STABLE_UUID, BLECharacteristic::PROPERTY_WRITE);
    motorCharStable->setCallbacks(new MotorCharCallbacks());
    
    auto* serialNumberChar = this->motorService->createCharacteristic(BH_BLE_SERVICE_MOTOR_CHAR_SERIAL_KEY_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE_NR);
    serialNumberChar->setValue(BH_SERIAL_NUMBER);
    serialNumberChar->setCallbacks(new SerialOutputCharCallbacks());
    
    auto* configChar = this->motorService->createCharacteristic(BH_BLE_SERVICE_MOTOR_CHAR_CONFIG_UUID, BLECharacteristic::PROPERTY_READ);
    
    auto* batteryChar = this->motorService->createCharacteristic(BH_BLE_SERVICE_MOTOR_CHAR_BATTERY_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    batteryChar->addDescriptor(new BLE2902());
    uint16_t batteryLevel = 100;
    batteryChar->setValue(batteryLevel);

    auto* versionChar = this->motorService->createCharacteristic(BH_BLE_SERVICE_MOTOR_CHAR_VERSION_UUID, BLECharacteristic::PROPERTY_READ);
    uint16_t firmwareVersion = BH_FIRMWARE_VERSION;
    versionChar->setValue(firmwareVersion);
    
    auto* monitorChar = this->motorService->createCharacteristic(BH_BLE_SERVICE_MOTOR_CHAR_TACTSUIT_MONITOR_UUID, BLECharacteristic::PROPERTY_READ);
    auto* athGlobalChar = this->motorService->createCharacteristic(BH_BLE_SERVICE_MOTOR_CHAR_ATH_GLOBAL_CONF_UUID, BLECharacteristic::PROPERTY_READ);

    this->motorService->start();
    
    advertising->start();
}
