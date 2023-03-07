#pragma once

#include <abstract_connection.hpp>
#include <events.hpp>

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include <abstract_battery.hpp>
#endif

#include <string>
#include <BLEDevice.h>

namespace OH
{
  class ConnectionBLE : public AbstractConnection {
   private:
#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
    unsigned long lastBatteryUpdate = 0;
    BLEService* batteryService = nullptr;
    BLECharacteristic* batteryLevelCharacteristic = nullptr;
#endif

   protected:
    std::string deviceName;
    IEventDispatcher* dispatcher;
    BLEServer* bleServer = nullptr;

   public:
    ConnectionBLE(const std::string& deviceName, IEventDispatcher* dispatcher);
    void setup(void) override;
    void handleEvent(const IEvent* event) const override;

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
    virtual void handleBatteryChange(const BatteryLevelEvent* event) const;
#endif
  };
} // namespace OH
