#pragma once

#include <BLEUUID.h>

// All below are weird choices of bHaptics engineers...
// Why to use unconventional UUIDs, that are reserved for other purposes?
// You have an unlimited amount of other UUIDs

/**
 * Main service for bHaptics BLE devices.
 *
 * Nordic UART Service (NUS)
 * @see https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/libraries/bluetooth_services/services/nus.html
 */
#define BH_BLE_SERVICE_MOTOR_UUID BLEUUID("6e400001-b5a3-f393-e0a9-e50e24dcca9e")

/**
 * Legacy Characteristic for driving haptic feedback.
 *
 * Nordic UART RX
 * @see https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/libraries/bluetooth_services/services/nus.html
 */
#define BH_BLE_SERVICE_MOTOR_CHAR_MOTOR_UUID BLEUUID("6e400002-b5a3-f393-e0a9-e50e24dcca9e")

/**
 * Characteristic for Device S/N.
 *
 * Nordic UART TX
 * @see https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/libraries/bluetooth_services/services/nus.html
 */
#define BH_BLE_SERVICE_MOTOR_CHAR_SERIAL_KEY_UUID BLEUUID("6e400003-b5a3-f393-e0a9-e50e24dcca9e")

// Glow Color
#define BH_BLE_SERVICE_MOTOR_CHAR_CONFIG_UUID BLEUUID("6e400005-b5a3-f393-e0a9-e50e24dcca9e")
#define BH_BLE_SERVICE_MOTOR_CHAR_VERSION_UUID BLEUUID("6e400007-b5a3-f393-e0a9-e50e24dcca9e")
#define BH_BLE_SERVICE_MOTOR_CHAR_BATTERY_UUID BLEUUID("6e400008-b5a3-f393-e0a9-e50e24dcca9e")

// Current Characteristic for driving haptic feedback
#define BH_BLE_SERVICE_MOTOR_CHAR_MOTOR_STABLE_UUID BLEUUID("6e40000a-b5a3-f393-e0a9-e50e24dcca9e")
#define BH_BLE_SERVICE_MOTOR_CHAR_TACTSUIT_MONITOR_UUID BLEUUID("6e40000b-b5a3-f393-e0a9-e50e24dcca9e")

// Audio-to-Haptic
#define BH_BLE_SERVICE_MOTOR_CHAR_ATH_GLOBAL_CONF_UUID BLEUUID("6e40000c-b5a3-f393-e0a9-e50e24dcca9e")
// Audio-to-Haptic
#define BH_BLE_SERVICE_MOTOR_CHAR_ATH_THEME_UUID BLEUUID("6e40000d-b5a3-f393-e0a9-e50e24dcca9e")

#define BH_BLE_SERVICE_MOTOR_CHAR_MOTTOR_MAPPING_UUID BLEUUID("6e40000e-b5a3-f393-e0a9-e50e24dcca9e")
#define BH_BLE_SERVICE_MOTOR_CHAR_SIGNATURE_PATTERN_UUID BLEUUID("6e40000f-b5a3-f393-e0a9-e50e24dcca9e")

/**
 * Firmware update service
 * @see https://infocenter.nordicsemi.com/topic/ug_nrfconnect_ble/UG/nRF_Connect_BLE/nRF_Connect_DFU.html
 */
#define BH_BLE_SERVICE_DFU_UUID BLEUUID("0000fe59-0000-1000-8000-00805f9b34fb")
/**
 * Firmware update control characteristic
 * @see https://infocenter.nordicsemi.com/topic/ug_nrfconnect_ble/UG/nRF_Connect_BLE/nRF_Connect_DFU.html
 */
#define BH_BLE_SERVICE_DFU_CHAR_CONTROL_UUID BLEUUID("8ec90003-f315-4f60-9fb8-838830daea50")
