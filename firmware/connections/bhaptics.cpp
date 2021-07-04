#include <HardwareSerial.h>
#include <BLE2902.h>

#include "config/bhaptics.h"

#include "connections/bhaptics.h"
#include "output.h"

class BHServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        Serial.println(">>\tonConnect()");
    }

    void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t* param) {
        Serial.println(">>\tonConnect(*param)");
    }

    void onDisconnect(BLEServer* pServer) {
        Serial.println(">>\tonDisconnect()");
        pServer->startAdvertising();
    }
};

class SerialOutputCharCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic) override
    {
        Serial.printf(">>\tonWrite (UUID: %s) \n", pCharacteristic->getUUID().toString().c_str());
        Serial.printf("\tvalue: `%s`, len: %u \n", pCharacteristic->getValue().c_str(), pCharacteristic->getValue().length());
    };

    void onRead(BLECharacteristic *pCharacteristic) override
    {
        Serial.printf(">>\tonRead (UUID: %s) \n", pCharacteristic->getUUID().toString().c_str());
        Serial.printf("\tvalue: `%s`, len: %u \n", pCharacteristic->getValue().c_str(), pCharacteristic->getValue().length());
    };

    void onNotify(BLECharacteristic* pCharacteristic) override
    {
        Serial.printf(">>\tonNotify (UUID: %s) \n", pCharacteristic->getUUID().toString().c_str());
        Serial.printf("\tvalue: `%s`, len: %u \n", pCharacteristic->getValue().c_str(), pCharacteristic->getValue().length());
    };

	void onStatus(BLECharacteristic* pCharacteristic, Status s, uint32_t code) override
    {
        Serial.printf(">>\tonStatus (UUID: %s) \n", pCharacteristic->getUUID().toString().c_str());
        Serial.printf("\tstatus: %d, code: %u \n", s, code);
        Serial.printf("\tvalue: `%s`, len: %u \n", pCharacteristic->getValue().c_str(), pCharacteristic->getValue().length());
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

        //todo: write right to the App.getOutput()
    };
};

class ConfigCharCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic) override
    {
        auto value = pCharacteristic->getValue();

        if (value.length() != 3) {
            return;
        }

        auto byte_0 = value[0],
             byte_1 = value[1],
             byte_2 = value[2];

        Serial.printf(">>\tonWrite (Config Char): %3hhu %2hhu %2hhu \n", byte_0, byte_1, byte_2);
    };

    void onRead(BLECharacteristic *pCharacteristic) override
    {
        auto value = pCharacteristic->getValue();

        if (value.length() != 3) {
            return;
        }

        auto byte_0 = value[0],
             byte_1 = value[1],
             byte_2 = value[2];

        Serial.printf(">>\tonRead (Config Char): %3hhu %2hhu %2hhu \n", byte_0, byte_1, byte_2);
    };
};

void BHapticsBLEConnection::setup()
{
    BLEConnection::setup();
    this->bleServer->setCallbacks(new BHServerCallbacks());
    auto advertising = this->bleServer->getAdvertising();

    auto scanResponseData = new BLEAdvertisementData();
    scanResponseData->setAppearance(BH_BLE_APPEARANCE);
    scanResponseData->setName(BLUETOOTH_NAME);

    advertising->setScanResponseData(*scanResponseData);

    advertising->setAppearance(BH_BLE_APPEARANCE);
    advertising->addServiceUUID(BH_BLE_SERVICE_MOTOR_UUID);
    advertising->addServiceUUID(BH_BLE_SERVICE_DFU_UUID);

    // Each characteristic needs 2 handles and descriptor 1 handle.
    this->motorService = this->bleServer->createService(BH_BLE_SERVICE_MOTOR_UUID, 25);

    auto* motorChar = this->motorService->createCharacteristic(
        BH_BLE_SERVICE_MOTOR_CHAR_MOTOR_UUID, 
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_BROADCAST| BLECharacteristic::PROPERTY_INDICATE| BLECharacteristic::PROPERTY_WRITE_NR
    );
    motorChar->setCallbacks(new MotorCharCallbacks());

    auto* motorCharStable = this->motorService->createCharacteristic(
        BH_BLE_SERVICE_MOTOR_CHAR_MOTOR_STABLE_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_BROADCAST| BLECharacteristic::PROPERTY_INDICATE| BLECharacteristic::PROPERTY_WRITE_NR
    );
    motorCharStable->setCallbacks(new MotorCharCallbacks());
    
    auto* configChar = this->motorService->createCharacteristic(
        BH_BLE_SERVICE_MOTOR_CHAR_CONFIG_UUID, 
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_BROADCAST| BLECharacteristic::PROPERTY_INDICATE| BLECharacteristic::PROPERTY_WRITE_NR
    );
    configChar->setCallbacks(new ConfigCharCallbacks());
    
    auto* serialNumberChar = this->motorService->createCharacteristic(
        BH_BLE_SERVICE_MOTOR_CHAR_SERIAL_KEY_UUID, 
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_BROADCAST| BLECharacteristic::PROPERTY_INDICATE| BLECharacteristic::PROPERTY_WRITE_NR
    );
    uint8_t serialNumber[] = BH_SERIAL_NUMBER;
    serialNumberChar->setValue(serialNumber, 10);
    serialNumberChar->setCallbacks(new SerialOutputCharCallbacks());
    
    auto* batteryChar = this->motorService->createCharacteristic(
        BH_BLE_SERVICE_MOTOR_CHAR_BATTERY_UUID, 
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_BROADCAST| BLECharacteristic::PROPERTY_INDICATE| BLECharacteristic::PROPERTY_WRITE_NR
    );
    batteryChar->setCallbacks(new SerialOutputCharCallbacks());
    batteryChar->addDescriptor(new BLE2902());
    uint16_t batteryLevel = 100;
    batteryChar->setValue(batteryLevel);

    auto* versionChar = this->motorService->createCharacteristic(
        BH_BLE_SERVICE_MOTOR_CHAR_VERSION_UUID, 
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_BROADCAST| BLECharacteristic::PROPERTY_INDICATE| BLECharacteristic::PROPERTY_WRITE_NR
    );
    versionChar->setCallbacks(new SerialOutputCharCallbacks());
    uint16_t firmwareVersion = BH_FIRMWARE_VERSION;
    versionChar->setValue(firmwareVersion);
    
    auto* monitorChar = this->motorService->createCharacteristic(
        BH_BLE_SERVICE_MOTOR_CHAR_TACTSUIT_MONITOR_UUID, 
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_BROADCAST| BLECharacteristic::PROPERTY_INDICATE| BLECharacteristic::PROPERTY_WRITE_NR
    );
    monitorChar->setCallbacks(new SerialOutputCharCallbacks());
    
    auto* athGlobalChar = this->motorService->createCharacteristic(
        BH_BLE_SERVICE_MOTOR_CHAR_ATH_GLOBAL_CONF_UUID, 
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_BROADCAST| BLECharacteristic::PROPERTY_INDICATE| BLECharacteristic::PROPERTY_WRITE_NR
    );
    athGlobalChar->setCallbacks(new SerialOutputCharCallbacks());
    
    auto* athThemeChar = this->motorService->createCharacteristic(
        BH_BLE_SERVICE_MOTOR_CHAR_ATH_THEME_UUID, 
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_BROADCAST| BLECharacteristic::PROPERTY_INDICATE| BLECharacteristic::PROPERTY_WRITE_NR
    );
    athThemeChar->setCallbacks(new SerialOutputCharCallbacks());
    
    auto* motorMappingChar = this->motorService->createCharacteristic(
        BH_BLE_SERVICE_MOTOR_CHAR_MOTTOR_MAPPING_UUID, 
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_BROADCAST| BLECharacteristic::PROPERTY_INDICATE| BLECharacteristic::PROPERTY_WRITE_NR
    );
    motorMappingChar->setCallbacks(new SerialOutputCharCallbacks());
    
    auto* signatureMappingChar = this->motorService->createCharacteristic(
        BH_BLE_SERVICE_MOTOR_CHAR_SIGNATURE_PATTERN_UUID, 
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_BROADCAST| BLECharacteristic::PROPERTY_INDICATE| BLECharacteristic::PROPERTY_WRITE_NR
    );
    signatureMappingChar->setCallbacks(new SerialOutputCharCallbacks());

    // auto dfuService = this->bleServer->createService(BH_BLE_SERVICE_DFU_UUID);

    // auto* dfuControlChar = dfuService->createCharacteristic(
    //     BH_BLE_SERVICE_MOTOR_CHAR_SIGNATURE_PATTERN_UUID, 
    //     BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_BROADCAST| BLECharacteristic::PROPERTY_INDICATE| BLECharacteristic::PROPERTY_WRITE_NR
    // );
    // dfuControlChar->setCallbacks(new SerialOutputCharCallbacks());
    // dfuService->start();

    this->motorService->start();
    
    advertising->start();
}
