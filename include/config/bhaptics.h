#pragma once

#ifdef BHAPTICS

#include "config/bluetooth.h"

// All below are weird choices of bHaptics engineers...
// Why to use unconventional UUIDs, that are reserved for other purposes?
// You have  an unlimited amount of other UUIDs
#define BH_BLE_SERVICE_MOTOR_UUID                 BLEUUID("6e400001-b5a3-f393-e0a9-e50e24dcca9e") // Nordic UART Service
#define BH_BLE_SERVICE_MOTOR_CHAR_MOTOR_UUID      BLEUUID("6e400002-b5a3-f393-e0a9-e50e24dcca9e") // Nordic UART RX
#define BH_BLE_SERVICE_MOTOR_CHAR_SERIAL_KEY_UUID BLEUUID("6e400003-b5a3-f393-e0a9-e50e24dcca9e") // Nordic UART TX

#endif
