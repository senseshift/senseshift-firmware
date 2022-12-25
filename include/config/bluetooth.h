#pragma once

#ifndef BLUETOOTH_NAME
#define BLUETOOTH_NAME "OpenHaptics"
#endif

#ifndef BLUETOOTH_ADDRESS
#define BLUETOOTH_ADDRESS { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB }
#endif

#ifndef BLE_BATTERY_SERVICE_UUID
#define BLE_BATTERY_SERVICE_UUID BLEUUID((uint16_t)0x180F)
#endif

#ifndef BLE_BATTERY_SERVICE_CHAR_LEVEL_UUID
#define BLE_BATTERY_SERVICE_CHAR_LEVEL_UUID BLEUUID((uint16_t)0x2A19)
#endif

