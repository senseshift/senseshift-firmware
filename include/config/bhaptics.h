#pragma once

#ifdef BHAPTICS

#ifndef BH_BLE_APPEARANCE
#define BH_BLE_APPEARANCE 508
#endif

#ifndef BH_SERIAL_NUMBER
#define BH_SERIAL_NUMBER "3130-3031-2d33-6437-642d"
#endif

#ifndef BH_FIRMWARE_VERSION
#define BH_FIRMWARE_VERSION (uint16_t) 510
#endif

#include "config/bluetooth.h"

// All below are weird choices of bHaptics engineers...
// Why to use unconventional UUIDs, that are reserved for other purposes?
// You have  an unlimited amount of other UUIDs
#define BH_BLE_SERVICE_MOTOR_UUID                          BLEUUID("6e400001-b5a3-f393-e0a9-e50e24dcca9e") // Nordic UART Service
#define BH_BLE_SERVICE_MOTOR_CHAR_MOTOR_UUID               BLEUUID("6e400002-b5a3-f393-e0a9-e50e24dcca9e") // Nordic UART RX
#define BH_BLE_SERVICE_MOTOR_CHAR_SERIAL_KEY_UUID          BLEUUID("6e400003-b5a3-f393-e0a9-e50e24dcca9e") // Nordic UART TX
#define BH_BLE_SERVICE_MOTOR_CHAR_CONFIG_UUID              BLEUUID("6e400005-b5a3-f393-e0a9-e50e24dcca9e")
#define BH_BLE_SERVICE_MOTOR_CHAR_BATTERY_UUID             BLEUUID("6e400008-b5a3-f393-e0a9-e50e24dcca9e")
#define BH_BLE_SERVICE_MOTOR_CHAR_VERSION_UUID             BLEUUID("6e400007-b5a3-f393-e0a9-e50e24dcca9e")
#define BH_BLE_SERVICE_MOTOR_CHAR_MOTOR_STABLE_UUID        BLEUUID("6e40000a-b5a3-f393-e0a9-e50e24dcca9e")
#define BH_BLE_SERVICE_MOTOR_CHAR_TACTSUIT_MONITOR_UUID    BLEUUID("6e40000b-b5a3-f393-e0a9-e50e24dcca9e")
#define BH_BLE_SERVICE_MOTOR_CHAR_ATH_GLOBAL_CONF_UUID     BLEUUID("6e40000c-b5a3-f393-e0a9-e50e24dcca9e") // Audio-to-Haptic
#define BH_BLE_SERVICE_MOTOR_CHAR_ATH_THEME_UUID           BLEUUID("6e40000d-b5a3-f393-e0a9-e50e24dcca9e") // Audio-to-Haptic
#define BH_BLE_SERVICE_MOTOR_CHAR_MOTTOR_MAPPING_UUID      BLEUUID("6e40000e-b5a3-f393-e0a9-e50e24dcca9e")
#define BH_BLE_SERVICE_MOTOR_CHAR_SIGNATURE_PATTERN_UUID   BLEUUID("6e40000f-b5a3-f393-e0a9-e50e24dcca9e")

#endif
