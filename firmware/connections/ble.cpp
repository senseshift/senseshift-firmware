#include <Arduino.h>
#include <BLEDevice.h>

#include "connections/ble.h"
#include "openhaptics.h"

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include <BLE2902.h>
#endif

void BLEConnection::setup() {
  BLEDevice::init(this->deviceName);

  this->bleServer = BLEDevice::createServer();

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  this->batteryService =
      this->bleServer->createService(BLE_BATTERY_SERVICE_UUID);
  this->batteryLevelCharacteristic = this->batteryService->createCharacteristic(
      BLE_BATTERY_SERVICE_CHAR_LEVEL_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  this->batteryLevelCharacteristic->addDescriptor(new BLE2902());

  uint16_t level = map(App.getBattery()->getLevel(), 0, 255, 0, 100);
  this->batteryLevelCharacteristic->setValue(level);

  this->batteryService->start();
#endif

  this->bleServer->getAdvertising()->start();
}

void BLEConnection::loop() {
  auto now_ms = millis();

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  if (now_ms - this->lastBatteryUpdate >= BATTERY_SAMPLE_RATE) {
    this->lastBatteryUpdate = now_ms;
    uint16_t level = map(App.getBattery()->getLevel(), 0, 255, 0, 100);

    this->batteryLevelCharacteristic->setValue(level);
    this->batteryLevelCharacteristic->notify();
  }
#endif
}
