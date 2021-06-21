
#define OH_MODE MODE_BHAPTICS_X16 // Set desired mode here

// Modes Configuration

#define MODE_BHAPTICS_X16 1
    #if OH_MODE == MODE_BHAPTICS_X16
        #define OH_CONNECTION   CONNECTION_BLE
        #define OH_OUTPUT       OUTPUT_VEST_X16
        #define BLUETOOTH_NAME  "TactSuit X16"
    #endif

/* #region Output Configuration */

#define OUTPUT_VEST_X8 0 // todo

#define OUTPUT_VEST_X16 1
    #define OUTPUT_DRIVER_ADDRESS_0 0x40

    #define O_V_X16_M0  0
    #define O_V_X16_M1  1
    #define O_V_X16_M2  2
    #define O_V_X16_M3  3
    #define O_V_X16_M4  4
    #define O_V_X16_M5  5
    #define O_V_X16_M6  6
    #define O_V_X16_M7  7
    #define O_V_X16_M8  8
    #define O_V_X16_M9  9
    #define O_V_X16_M10 10
    #define O_V_X16_M11 11
    #define O_V_X16_M12 12
    #define O_V_X16_M13 13
    #define O_V_X16_M14 14
    #define O_V_X16_M15 15

/* #endregion */

// Connection Configuration

#define CONNECTION_BLE 1
    // https://gist.github.com/sam016/4abe921b5a9ee27f67b3686910293026
    // https://github.com/oesmith/gatt-xml
    // https://www.bluetooth.com/specifications/assigned-numbers/

    #define BLE_SERVICE_BATTERY_UUID                        BLEUUID((uint16_t)0x180F)
    #define BLE_SERVICE_BATTERY_CHAR_BATTERY_LEVEL_UUID     BLEUUID((uint16_t)0x2A19)

    #define BLE_SERVICE_MOTOR_UUID                          BLEUUID("6e400001-b5a3-f393-e0a9-e50e24dcca9e")
    #define BLE_SERVICE_MOTOR_CHAR_MOTOR_UUID               BLEUUID("6e400002-b5a3-f393-e0a9-e50e24dcca9e")
    #define BLE_SERVICE_MOTOR_CHAR_SERIAL_KEY_UUID          BLEUUID("6e400003-b5a3-f393-e0a9-e50e24dcca9e")
    #define BLE_SERVICE_MOTOR_CHAR_CONFIG_UUID              BLEUUID("6e400005-b5a3-f393-e0a9-e50e24dcca9e")
    #define BLE_SERVICE_MOTOR_CHAR_BATTERY_UUID             BLEUUID("6e400008-b5a3-f393-e0a9-e50e24dcca9e")
    #define BLE_SERVICE_MOTOR_CHAR_VERSION_UUID             BLEUUID("6e400007-b5a3-f393-e0a9-e50e24dcca9e")
    #define BLE_SERVICE_MOTOR_CHAR_MOTOR_STABLE_UUID        BLEUUID("6e40000a-b5a3-f393-e0a9-e50e24dcca9e")
    #define BLE_SERVICE_MOTOR_CHAR_TACTSUIT_MONITOR_UUID    BLEUUID("6e40000b-b5a3-f393-e0a9-e50e24dcca9e")
    #define BLE_SERVICE_MOTOR_CHAR_ATH_GLOBAL_CONF_UUID     BLEUUID("6e40000c-b5a3-f393-e0a9-e50e24dcca9e")
    #define BLE_SERVICE_MOTOR_CHAR_ATH_THEME_UUID           BLEUUID("6e40000d-b5a3-f393-e0a9-e50e24dcca9e")
    #define BLE_SERVICE_MOTOR_CHAR_MOTTOR_MAPPING_UUID      BLEUUID("6e40000e-b5a3-f393-e0a9-e50e24dcca9e")
    #define BLE_SERVICE_MOTOR_CHAR_SIGNATURE_PATTERN_UUID   BLEUUID("6e40000f-b5a3-f393-e0a9-e50e24dcca9e")
    
    #define BLE_SERVICE_DFU_UUID                BLEUUID("0000fe59-0000-1000-8000-00805f9b34fb")
    #define BLE_SERVICE_DFU_CHAR_CONTROL_UUID   BLEUUID("8ec90003-f315-4f60-9fb8-838830daea50")

// Bluetooth Common Settings

#ifndef BLUETOOTH_NAME
    #define BLUETOOTH_NAME "TactSuit X16" // Fallback
#endif
