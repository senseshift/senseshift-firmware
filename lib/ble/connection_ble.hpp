#pragma once

#include <abstract_connection.hpp>

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
    BLEServer* bleServer = nullptr;

   public:
    ConnectionBLE(const std::string& deviceName) : deviceName(deviceName){};
    void setup(void) override;
    void loop(void) override;
  };
} // namespace OH
