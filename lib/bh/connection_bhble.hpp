#pragma once

#include <bh_constants.h>
#include <bh_types.hpp>

#include <connection_ble.hpp>
#include <esp_wifi.h>

typedef void (*bh_motor_transformer_t)(std::string&);

namespace BH
{
  class ConnectionBHBLE final : public OH::ConnectionBLE {
   private:
    BLEService* motorService = nullptr;

    unsigned long lastBatteryUpdate = 0;
    BLECharacteristic* batteryChar = nullptr;

   protected:
    uint16_t appearance;
    uint8_t* serialNumber;
    bh_motor_transformer_t motorTransformer;

   public:
    ConnectionBHBLE(const std::string& deviceName, const uint16_t appearance, uint8_t* serialNumber, bh_motor_transformer_t motorTransformer)
      : ConnectionBLE(deviceName),
        appearance(appearance),
        serialNumber(serialNumber),
        motorTransformer(motorTransformer) {};
    void setup(void) override;
  };
} // namespace OH
