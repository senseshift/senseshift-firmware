#pragma once

#ifdef BHAPTICS

#include "config/bluetooth.h"

#ifndef BH_BLE_APPEARANCE
#define BH_BLE_APPEARANCE 508
#endif

#ifndef BH_SERIAL_NUMBER
#define BH_SERIAL_NUMBER                                       \
  {                                                            \
    0x76, 0xbd, 0xe8, 0xe7, 0x83, 0x5d, 0x8b, 0x71, 0x38, 0x1c \
  }  // 76b5-e8e7-835d-8b71-381c
#endif

#ifndef BH_FIRMWARE_VERSION
#define BH_FIRMWARE_VERSION (uint16_t)512
#endif

#define NO_AUDIO_CABLE 0
#define AUDIO_CABLE 1

// All below are weird choices of bHaptics engineers...
// Why to use unconventional UUIDs, that are reserved for other purposes?
// You have  an unlimited amount of other UUIDs

// Main service for communication
#define BH_BLE_SERVICE_MOTOR_UUID \
  BLEUUID("6e400001-b5a3-f393-e0a9-e50e24dcca9e")  // Nordic UART Service

// Legacy Characteristic to create haptic feedback
#define BH_BLE_SERVICE_MOTOR_CHAR_MOTOR_UUID \
  BLEUUID("6e400002-b5a3-f393-e0a9-e50e24dcca9e")  // Nordic UART RX

// Characteristic for Device S/N
#define BH_BLE_SERVICE_MOTOR_CHAR_SERIAL_KEY_UUID \
  BLEUUID("6e400003-b5a3-f393-e0a9-e50e24dcca9e")  // Nordic UART TX

// Glow Color
#define BH_BLE_SERVICE_MOTOR_CHAR_CONFIG_UUID \
  BLEUUID("6e400005-b5a3-f393-e0a9-e50e24dcca9e")
#define BH_BLE_SERVICE_MOTOR_CHAR_VERSION_UUID \
  BLEUUID("6e400007-b5a3-f393-e0a9-e50e24dcca9e")
#define BH_BLE_SERVICE_MOTOR_CHAR_BATTERY_UUID \
  BLEUUID("6e400008-b5a3-f393-e0a9-e50e24dcca9e")
#define BH_BLE_SERVICE_MOTOR_CHAR_MOTOR_STABLE_UUID \
  BLEUUID("6e40000a-b5a3-f393-e0a9-e50e24dcca9e")
#define BH_BLE_SERVICE_MOTOR_CHAR_TACTSUIT_MONITOR_UUID \
  BLEUUID("6e40000b-b5a3-f393-e0a9-e50e24dcca9e")
#define BH_BLE_SERVICE_MOTOR_CHAR_ATH_GLOBAL_CONF_UUID \
  BLEUUID("6e40000c-b5a3-f393-e0a9-e50e24dcca9e")  // Audio-to-Haptic
#define BH_BLE_SERVICE_MOTOR_CHAR_ATH_THEME_UUID \
  BLEUUID("6e40000d-b5a3-f393-e0a9-e50e24dcca9e")  // Audio-to-Haptic
#define BH_BLE_SERVICE_MOTOR_CHAR_MOTTOR_MAPPING_UUID \
  BLEUUID("6e40000e-b5a3-f393-e0a9-e50e24dcca9e")
#define BH_BLE_SERVICE_MOTOR_CHAR_SIGNATURE_PATTERN_UUID \
  BLEUUID("6e40000f-b5a3-f393-e0a9-e50e24dcca9e")

#define BH_BLE_SERVICE_DFU_UUID BLEUUID("0000fe59-0000-1000-8000-00805f9b34fb")
#define BH_BLE_SERVICE_DFU_CHAR_CONTROL_UUID \
  BLEUUID("8ec90003-f315-4f60-9fb8-838830daea50")

#endif
