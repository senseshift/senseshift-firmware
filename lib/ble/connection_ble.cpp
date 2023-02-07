#include "connection_ble.hpp"

#include "ble_constants.h"

#include <Arduino.h>
#include <BLEDevice.h>

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include "abstract_battery.hpp"
#include <BLE2902.h>
#endif

void OH::ConnectionBLE::setup() {
  BLEDevice::init(this->deviceName);

  this->bleServer = BLEDevice::createServer();

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  this->batteryService =
      this->bleServer->createService(BLE_BATTERY_SERVICE_UUID);
  this->batteryLevelCharacteristic = this->batteryService->createCharacteristic(
      BLE_BATTERY_SERVICE_CHAR_LEVEL_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  this->batteryLevelCharacteristic->addDescriptor(new BLE2902());

  this->batteryService->start();
#endif

  this->bleServer->getAdvertising()->start();
}
