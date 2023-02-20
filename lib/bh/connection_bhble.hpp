#pragma once

#include <bh_constants.h>
#include <bh_types.hpp>

#include <connection_ble.hpp>

#include <esp_wifi.h>

typedef void (*bh_motor_transformer_t)(std::string&);

namespace BH
{
  struct ConnectionBHBLE_Config {
    std::string deviceName;
    uint16_t appearance;
    uint8_t* serialNumber;
  };

  class ConnectionBHBLE final : public OH::ConnectionBLE {
   private:
    BLEService* motorService = nullptr;
    BLECharacteristic* batteryChar = nullptr;

   protected:
    uint16_t appearance;
    uint8_t* serialNumber;
    bh_motor_transformer_t motorTransformer;

   public:
    ConnectionBHBLE(ConnectionBHBLE_Config* config, bh_motor_transformer_t motorTransformer, OH::IEventDispatcher* eventDispatcher)
      : ConnectionBLE(config->deviceName, eventDispatcher),
        appearance(config->appearance),
        serialNumber(config->serialNumber),
        motorTransformer(motorTransformer) {};
    void setup(void) override;

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
    void handleBatteryChange(const OH::BatteryLevelEvent* event) const override;
#endif
  };
} // namespace OH
