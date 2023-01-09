#pragma once

#include "connection.h"
#include "connections/ble.h"

class BHapticsBLEConnection final : public BLEConnection {
 private:
  BLEService* motorService = nullptr;

  unsigned long lastBatteryUpdate = 0;
  BLECharacteristic* batteryChar = nullptr;

 protected:
  void (*motorTransformer)(std::string&);

 public:
  BHapticsBLEConnection(std::string deviceName,
                        void (*motorTransformer)(std::string&))
      : BLEConnection(deviceName), motorTransformer(motorTransformer){};
  void setup(void) override;
  void loop(void) override;
};
