#pragma once

#include <abstract_connection.hpp>
#include <connection_ble.hpp>

class BHapticsBLEConnection final : public OH::ConnectionBLE {
 private:
  BLEService* motorService = nullptr;

  unsigned long lastBatteryUpdate = 0;
  BLECharacteristic* batteryChar = nullptr;

 protected:
  void (*motorTransformer)(std::string&);

 public:
  BHapticsBLEConnection(OH::ConnectionBLEConfig deviceConfig, void (*motorTransformer)(std::string&)) : ConnectionBLE(deviceConfig), motorTransformer(motorTransformer){};
  void setup(void) override;
  void loop(void) override;
};
