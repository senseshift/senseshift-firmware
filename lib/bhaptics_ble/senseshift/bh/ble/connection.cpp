#include "senseshift/bh/ble/connection.hpp"

#include <haptic_body.hpp>
#include <senseshift/bh/constants.hpp>
#include <senseshift/events.hpp>

#include <Arduino.h>

#if defined(BLUETOOTH_USE_NIMBLE) && BLUETOOTH_USE_NIMBLE == true
// BLE2902 not needed: https://github.com/h2zero/NimBLE-Arduino/blob/release/1.4/docs/Migration_guide.md#descriptors

#define PROPERTY_READ NIMBLE_PROPERTY::READ
#define PROPERTY_WRITE NIMBLE_PROPERTY::WRITE
#define PROPERTY_WRITE_NR NIMBLE_PROPERTY::WRITE_NR
#define PROPERTY_BROADCAST NIMBLE_PROPERTY::BROADCAST
#define PROPERTY_NOTIFY NIMBLE_PROPERTY::NOTIFY
#define PROPERTY_INDICATE NIMBLE_PROPERTY::INDICATE
#else
#include <BLE2902.h>

#define PROPERTY_READ BLECharacteristic::PROPERTY_READ
#define PROPERTY_WRITE BLECharacteristic::PROPERTY_WRITE
#define PROPERTY_WRITE_NR BLECharacteristic::PROPERTY_WRITE_NR
#define PROPERTY_BROADCAST BLECharacteristic::PROPERTY_BROADCAST
#define PROPERTY_NOTIFY BLECharacteristic::PROPERTY_NOTIFY
#define PROPERTY_INDICATE BLECharacteristic::PROPERTY_INDICATE
#endif

namespace SenseShift::BH::BLE {
    class BHServerCallbacks final : public BLEServerCallbacks {
      private:
        ::SenseShift::IEventDispatcher* dispatcher;

      public:
        BHServerCallbacks(::SenseShift::IEventDispatcher* eventDispatcher) : dispatcher(eventDispatcher) {}

        void onConnect(BLEServer* pServer)
        {
            this->dispatcher->postEvent(new ::SenseShift::IEvent(OH_EVENT_CONNECTED));
        }

        void onDisconnect(BLEServer* pServer)
        {
            this->dispatcher->postEvent(new ::SenseShift::IEvent(OH_EVENT_DISCONNECTED));
            pServer->startAdvertising();
        }
    };

    class LogOutputCharCallbacks : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic* pCharacteristic) override
        {
            log_d(
              ">>\tonWrite (UUID: %s)\n\tvalue: `%s`, len: %u",
              pCharacteristic->getUUID().toString().c_str(),
              pCharacteristic->getValue().c_str(),
              pCharacteristic->getValue().length()
            );
        };

        void onRead(BLECharacteristic* pCharacteristic) override
        {
            log_d(
              ">>\tonRead (UUID: %s)\n\tvalue: `%s`, len: %u",
              pCharacteristic->getUUID().toString().c_str(),
              pCharacteristic->getValue().c_str(),
              pCharacteristic->getValue().length()
            );
        };

        void onNotify(BLECharacteristic* pCharacteristic) override
        {
            log_d(
              ">>\tonNotify (UUID: %s)\n\tvalue: `%s`, len: %u",
              pCharacteristic->getUUID().toString().c_str(),
              pCharacteristic->getValue().c_str(),
              pCharacteristic->getValue().length()
            );
        };

#if defined(BLUETOOTH_USE_NIMBLE) && BLUETOOTH_USE_NIMBLE == true
        void onStatus(BLECharacteristic* pCharacteristic, Status s, int code) override
#else
        void onStatus(BLECharacteristic* pCharacteristic, Status s, uint32_t code) override
#endif
        {
            log_d(
              ">>\tonNotify (UUID: %s)\n\tstatus: %d, code: %u \n\tvalue: `%s`, len: %u",
              pCharacteristic->getUUID().toString().c_str(),
              s,
              code,
              pCharacteristic->getValue().c_str(),
              pCharacteristic->getValue().length()
            );
        };
    };

    class MotorCharCallbacks : public BLECharacteristicCallbacks {
      private:
        Connection::MotorHandler_t motorTransformer;

      public:
        MotorCharCallbacks(Connection::MotorHandler_t motorTransformer) : motorTransformer(motorTransformer) {}

        void onWrite(BLECharacteristic* pCharacteristic) override
        {
            std::string value = pCharacteristic->getValue();

            this->motorTransformer(value);
        };
    };

    class ConfigCharCallbacks : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic* pCharacteristic) override
        {
            auto value = pCharacteristic->getValue();

            if (value.length() != 3) {
                return;
            }

            auto byte_0 = value[0], byte_1 = value[1], byte_2 = value[2];

            log_d(">>\tonWrite (Config Char): %3hhu %2hhu %2hhu", byte_0, byte_1, byte_2);
        };
    };

    void Connection::begin()
    {
        BLEDevice::init(this->config.deviceName);

        this->callbacks->postInit();

        this->bleServer = BLEDevice::createServer();

        this->bleServer->setCallbacks(new BHServerCallbacks(this->eventDispatcher));

        auto scanResponseData = new BLEAdvertisementData();
        scanResponseData->setAppearance(this->config.appearance);
        scanResponseData->setName(this->config.deviceName);

        this->bleServer->getAdvertising()->setAppearance(this->config.appearance);
        this->bleServer->getAdvertising()->setScanResponseData(*scanResponseData);

        // Each characteristic needs 2 handles and descriptor 1 handle.
        this->motorService = this->bleServer->createService(BH_BLE_SERVICE_MOTOR_UUID);

        {
            MotorCharCallbacks* motorCallbacks = new MotorCharCallbacks(this->motorHandler);

            auto* motorChar =
              this->motorService->createCharacteristic(BH_BLE_SERVICE_MOTOR_CHAR_MOTOR_UUID, PROPERTY_WRITE_NR);
            motorChar->setCallbacks(motorCallbacks);

            auto* motorCharStable =
              this->motorService->createCharacteristic(BH_BLE_SERVICE_MOTOR_CHAR_MOTOR_STABLE_UUID, PROPERTY_WRITE);
            motorCharStable->setCallbacks(motorCallbacks);
        }

        {
            auto* configChar = this->motorService->createCharacteristic(
              BH_BLE_SERVICE_MOTOR_CHAR_CONFIG_UUID,
              PROPERTY_READ | PROPERTY_WRITE
            );
            configChar->setCallbacks(new ConfigCharCallbacks());
        }

        {
            auto* serialNumberChar = this->motorService->createCharacteristic(
              BH_BLE_SERVICE_MOTOR_CHAR_SERIAL_KEY_UUID,
              PROPERTY_READ | PROPERTY_WRITE
            );
            serialNumberChar->setValue(this->config.serialNumber, ConnectionConfig_t::SN_LENGTH);
            serialNumberChar->setCallbacks(new LogOutputCharCallbacks());
        }

        {
            this->batteryChar = this->motorService->createCharacteristic(
              BH_BLE_SERVICE_MOTOR_CHAR_BATTERY_UUID,
              PROPERTY_READ | PROPERTY_WRITE_NR
                | PROPERTY_NOTIFY // for whatever reason, it have to be writable, otherwise Desktop app crashes
            );

#if !defined(BLUETOOTH_USE_NIMBLE) || BLUETOOTH_USE_NIMBLE != true
            batteryChar->addDescriptor(new BLE2902());
#endif

            // original bHaptics Player require non-null value for battery level, otherwise it crashes
#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
            uint16_t defaultLevel = 0;
#else
            uint16_t defaultLevel = 100;
#endif

            this->batteryChar->setValue(defaultLevel);
            // this->batteryChar->notify();
        }

        {
            auto* versionChar =
              this->motorService->createCharacteristic(BH_BLE_SERVICE_MOTOR_CHAR_VERSION_UUID, PROPERTY_READ);
            versionChar->setCallbacks(new LogOutputCharCallbacks());
            uint16_t firmwareVersion = BH_FIRMWARE_VERSION;
            versionChar->setValue(firmwareVersion);
        }

        {
            auto* monitorChar = this->motorService->createCharacteristic(
              BH_BLE_SERVICE_MOTOR_CHAR_TACTSUIT_MONITOR_UUID,
              PROPERTY_READ | PROPERTY_WRITE | PROPERTY_NOTIFY | PROPERTY_BROADCAST | PROPERTY_INDICATE
                | PROPERTY_WRITE_NR
            );
            monitorChar->setCallbacks(new LogOutputCharCallbacks());

#if !defined(BLUETOOTH_USE_NIMBLE) || BLUETOOTH_USE_NIMBLE != true
            monitorChar->addDescriptor(new BLE2902());
#endif

            uint16_t audioCableState = NO_AUDIO_CABLE;
            monitorChar->setValue(audioCableState);
        }

        // auto* athGlobalChar = this->motorService->createCharacteristic(
        //     BH_BLE_SERVICE_MOTOR_CHAR_ATH_GLOBAL_CONF_UUID,
        //     PROPERTY_READ | PROPERTY_WRITE | PROPERTY_NOTIFY | PROPERTY_BROADCAST | PROPERTY_INDICATE |
        //     PROPERTY_WRITE_NR
        // );
        // athGlobalChar->setCallbacks(new LogOutputCharCallbacks());

        // auto* athThemeChar = this->motorService->createCharacteristic(
        //     BH_BLE_SERVICE_MOTOR_CHAR_ATH_THEME_UUID,
        //     PROPERTY_READ | PROPERTY_WRITE | PROPERTY_NOTIFY | PROPERTY_BROADCAST | PROPERTY_INDICATE |
        //     PROPERTY_WRITE_NR
        // );
        // athThemeChar->setCallbacks(new LogOutputCharCallbacks());

        // auto* motorMappingChar = this->motorService->createCharacteristic(
        //     BH_BLE_SERVICE_MOTOR_CHAR_MOTTOR_MAPPING_UUID,
        //     PROPERTY_READ | PROPERTY_WRITE | PROPERTY_NOTIFY | PROPERTY_BROADCAST | PROPERTY_INDICATE |
        //     PROPERTY_WRITE_NR
        // );
        // motorMappingChar->setCallbacks(new LogOutputCharCallbacks());

        // auto* signatureMappingChar = this->motorService->createCharacteristic(
        //     BH_BLE_SERVICE_MOTOR_CHAR_SIGNATURE_PATTERN_UUID,
        //     PROPERTY_READ | PROPERTY_WRITE | PROPERTY_NOTIFY | PROPERTY_BROADCAST | PROPERTY_INDICATE |
        //     PROPERTY_WRITE_NR
        // );
        // signatureMappingChar->setCallbacks(new LogOutputCharCallbacks());

        this->motorService->start();

        {
            auto dfuService = this->bleServer->createService(BH_BLE_SERVICE_DFU_UUID);

            auto* dfuControlChar = dfuService->createCharacteristic(
              BH_BLE_SERVICE_MOTOR_CHAR_SIGNATURE_PATTERN_UUID,
              PROPERTY_READ | PROPERTY_WRITE
            );
            dfuService->start();
        }

        this->bleServer->getAdvertising()->start();
    }
} // namespace SenseShift::BH::BLE
