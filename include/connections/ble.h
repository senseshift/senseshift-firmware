#pragma once

#include <string>

#include <BLEDevice.h>

#include "config/all.h"
#include <abstract_connection.hpp>

class BLEConnection : public OH::AbstractConnection {
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
  BLEConnection(const std::string& deviceName) : deviceName(deviceName){};
  void setup(void) override;
  void loop(void) override;
};
