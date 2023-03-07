#include "connection_ble.hpp"

#include "ble_constants.h"

#include <Arduino.h>
#include <BLEDevice.h>

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include "abstract_battery.hpp"
#include <BLE2902.h>
#endif

OH::ConnectionBLE::ConnectionBLE(const std::string& deviceName, OH::IEventDispatcher* dispatcher)
  : deviceName(deviceName), dispatcher(dispatcher) {
#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  dispatcher->addEventListener(this);
#endif
}

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

void OH::ConnectionBLE::handleEvent(const OH::IEvent* event) const {
#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  if (event->eventName == OH_EVENT_BATTERY_LEVEL) {
    this->handleBatteryChange(static_cast<const OH::BatteryLevelEvent*>(event));
  }
#endif
}

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
void OH::ConnectionBLE::handleBatteryChange(const OH::BatteryLevelEvent* event) const {
  uint16_t level = map(event->level, 0, 255, 0, 100);

  this->batteryLevelCharacteristic->setValue(level);
  this->batteryLevelCharacteristic->notify();
}
#endif
