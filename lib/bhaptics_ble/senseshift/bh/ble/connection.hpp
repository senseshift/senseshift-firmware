#pragma once

#include <senseshift/battery.hpp>
#include <senseshift/bh/ble/constants.hpp>
#include <senseshift/bh/constants.hpp>
#include <senseshift/events.hpp>
#include <senseshift/utility.hpp>

#include <Arduino.h>
#include <esp_wifi.h>

#if defined(BLUETOOTH_USE_NIMBLE) && BLUETOOTH_USE_NIMBLE == true
#include <NimBLEDevice.h>
#else
#include <BLEDevice.h>
#endif

namespace SenseShift::BH::BLE {
    typedef struct ConnectionConfig {
        static constexpr size_t SN_LENGTH = 10;

        std::string deviceName;
        uint16_t appearance;
        uint8_t serialNumber[SN_LENGTH];
    } ConnectionConfig_t;

    class ConnectionCallbacks {
      public:
        virtual void postInit()
        {
            log_v("Default postInit");
        };
    };
    static ConnectionCallbacks defaultCallback;

    class Connection final : public IEventListener {
      public:
        typedef std::function<void(std::string&)> MotorHandler_t;

        Connection(const ConnectionConfig_t& config, MotorHandler_t motorHandler, IEventDispatcher* eventDispatcher) :
          config(config), motorHandler(motorHandler), eventDispatcher(eventDispatcher)
        {
            this->eventDispatcher->addEventListener(this);
        };

        void begin(void);
        void handleEvent(const IEvent* event) const override
        {
            if (event->eventName == OH_EVENT_BATTERY_LEVEL) {
                uint16_t level = simpleMap<uint8_t>(
                  static_cast<const ::SenseShift::Battery::BatteryLevelEvent*>(event)->state.level,
                  255,
                  100
                );

                this->batteryChar->setValue(level);
                this->batteryChar->notify();

                return;
            }
        };

        void setCallbacks(ConnectionCallbacks* pCallbacks)
        {
            if (pCallbacks != nullptr) {
                this->callbacks = pCallbacks;
            } else {
                this->callbacks = &defaultCallback;
            }
        };

      private:
        const ConnectionConfig_t& config;
        MotorHandler_t motorHandler;
        ::SenseShift::IEventDispatcher* eventDispatcher;

        BLEServer* bleServer = nullptr;
        BLEService* motorService = nullptr;
        BLECharacteristic* batteryChar = nullptr;

        ConnectionCallbacks* callbacks = &defaultCallback;
    };
} // namespace SenseShift::BH::BLE
