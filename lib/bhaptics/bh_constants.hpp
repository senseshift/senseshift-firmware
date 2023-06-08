#pragma once

#if defined(ESP32)
  #include <BLEUUID.h>
#endif

#define BH_SERIAL_NUMBER_LENGTH 10

#ifndef BH_FIRMWARE_VERSION
#define BH_FIRMWARE_VERSION (uint16_t)UINT16_MAX
#endif

#define NO_AUDIO_CABLE 0
#define AUDIO_CABLE 1

#pragma region BH_DEVICE_TACTSUITX16

#define BH_LAYOUT_TACTSUITX16_SIZE_X 4
#define BH_LAYOUT_TACTSUITX16_SIZE_Y 2
#define BH_LAYOUT_TACTSUITX16_MAKE_POINT(x, y) OH::PlaneMapper_Margin::mapPoint<oh_output_coord_t>(x, y, (oh_output_coord_t) (BH_LAYOUT_TACTSUITX16_SIZE_X - 1), (oh_output_coord_t) (BH_LAYOUT_TACTSUITX16_SIZE_Y - 1))

// X16 suit uses the same packets structure as x40 suit and performs motor grouping in firmware
#define BH_LAYOUT_TACTSUITX16_SIZE 40
#define BH_LAYOUT_TACTSUITX16 {                                 \
    /* Front, left part */                                      \
    /*  0 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 0),  /*  0 */  \
    /*  1 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 0),  /*  1 */  \
    /*  2 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 0),  /*  4 */  \
    /*  3 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 0),  /*  5 */  \
                                                                \
    /*  4 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 1),  /*  8 */  \
    /*  5 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 1),  /*  9 */  \
    /*  6 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 1),  /* 12 */  \
    /*  7 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 1),  /* 13 */  \
    /*  8 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 1),  /* 16 */  \
    /*  9 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 1),  /* 17 */  \
                                                                \
    /* Back */                                                  \
    /* 10 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 0),  /*  0 */  \
    /* 11 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 0),  /*  1 */  \
    /* 12 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 0),  /*  4 */  \
    /* 13 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 0),  /*  5 */  \
                                                                \
    /* 14 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 1),  /*  8 */  \
    /* 15 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 1),  /*  9 */  \
    /* 16 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 1),  /* 12 */  \
    /* 17 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 1),  /* 13 */  \
    /* 18 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 1),  /* 16 */  \
    /* 19 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 1),  /* 17 */  \
                                                                \
    /* 20 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 0),  /*  2 */  \
    /* 21 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 0),  /*  3 */  \
    /* 22 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 0),  /*  4 */  \
    /* 23 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 0),  /*  7 */  \
                                                                \
    /* 24 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 1),  /* 10 */  \
    /* 25 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 1),  /* 11 */  \
    /* 26 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 1),  /* 14 */  \
    /* 27 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 1),  /* 15 */  \
    /* 28 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 1),  /* 18 */  \
    /* 29 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 1),  /* 19 */  \
                                                                \
    /* Front, again... Now right part */                        \
    /* 30 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 0),  /*  2 */  \
    /* 31 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 0),  /*  3 */  \
    /* 32 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 0),  /*  4 */  \
    /* 33 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 0),  /*  7 */  \
                                                                \
    /* 34 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 1),  /* 10 */  \
    /* 35 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 1),  /* 11 */  \
    /* 36 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 1),  /* 14 */  \
    /* 37 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 1),  /* 15 */  \
    /* 38 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 1),  /* 18 */  \
    /* 39 */ BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 1),  /* 19 */  \
}

// Ouput indices, responsible for x40 => x16 grouping
#define BH_LAYOUT_TACTSUITX16_GROUPS { 0, 1, 4, 5, 10, 11, 14, 15, 20, 21, 24, 25, 30, 31, 34, 35 }
#define BH_LAYOUT_TACTSUITX16_GROUPS_SIZE 16

#pragma endregion BH_DEVICE_TACTSUITX16

#pragma region BH_DEVICE_TACTSUITX40

#define BH_LAYOUT_TACTSUITX40_SIZE_X 4
#define BH_LAYOUT_TACTSUITX40_SIZE_Y 5
#define BH_LAYOUT_TACTSUITX40_MAKE_POINT(x, y) OH::PlaneMapper_Margin::mapPoint<oh_output_coord_t>(x, y, (oh_output_coord_t) (BH_LAYOUT_TACTSUITX40_SIZE_X - 1), (oh_output_coord_t) (BH_LAYOUT_TACTSUITX40_SIZE_Y - 1))

// X * Y for front and back
#define BH_LAYOUT_TACTSUITX40_SIZE 40
#define BH_LAYOUT_TACTSUITX40 {                       \
    /* Front, left part */                            \
    /*  0 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 0),  \
    /*  1 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 0),  \
    /*  2 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 1),  \
    /*  3 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 1),  \
    /*  4 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 2),  \
    /*  5 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 2),  \
    /*  6 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 3),  \
    /*  7 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 3),  \
    /*  8 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 4),  \
    /*  9 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 4),  \
                                                      \
    /* Back */                                        \
    /* 11 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 0),  \
    /* 11 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 0),  \
    /* 12 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 1),  \
    /* 13 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 1),  \
    /* 14 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 2),  \
    /* 15 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 2),  \
    /* 16 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 3),  \
    /* 17 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 3),  \
    /* 18 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 4),  \
    /* 19 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 4),  \
                                                      \
    /* 20 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 0),  \
    /* 21 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 0),  \
    /* 22 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 1),  \
    /* 23 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 1),  \
    /* 24 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 2),  \
    /* 25 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 2),  \
    /* 26 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 3),  \
    /* 27 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 3),  \
    /* 28 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 4),  \
    /* 29 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 4),  \
                                                      \
    /* Front, again... Now right part */              \
    /* 30 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 0),  \
    /* 31 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 0),  \
    /* 32 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 1),  \
    /* 33 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 1),  \
    /* 34 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 2),  \
    /* 35 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 2),  \
    /* 36 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 3),  \
    /* 37 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 3),  \
    /* 38 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 4),  \
    /* 39 */ BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 4)   \
}

#pragma endregion BH_DEVICE_TACTSUITX40

#pragma region BH_DEVICE_TACTAL

#define BH_LAYOUT_TACTAL_SIZE_X 6
#define BH_LAYOUT_TACTAL_SIZE_Y 1
#define BH_LAYOUT_TACTAL_MAKE_POINT(x, y) OH::PlaneMapper_Margin::mapPoint<oh_output_coord_t>(x, y, (oh_output_coord_t) (BH_LAYOUT_TACTAL_SIZE_X - 1), (oh_output_coord_t) (BH_LAYOUT_TACTAL_SIZE_Y - 1))

#define BH_LAYOUT_TACTAL_SIZE (BH_LAYOUT_TACTAL_SIZE_X * BH_LAYOUT_TACTAL_SIZE_Y)
#define BH_LAYOUT_TACTAL {           \
  BH_LAYOUT_TACTAL_MAKE_POINT(0, 0), \
  BH_LAYOUT_TACTAL_MAKE_POINT(1, 0), \
  BH_LAYOUT_TACTAL_MAKE_POINT(2, 0), \
  BH_LAYOUT_TACTAL_MAKE_POINT(3, 0), \
  BH_LAYOUT_TACTAL_MAKE_POINT(4, 0), \
  BH_LAYOUT_TACTAL_MAKE_POINT(5, 0), \
}

#pragma endregion BH_DEVICE_TACTVISOR

#pragma region BH_DEVICE_TACTAL

#define BH_LAYOUT_TACTVISOR_SIZE_X 4
#define BH_LAYOUT_TACTVISOR_SIZE_Y 1
#define BH_LAYOUT_TACTVISOR_MAKE_POINT(x, y) OH::PlaneMapper_Margin::mapPoint<oh_output_coord_t>(x, y, (oh_output_coord_t) (BH_LAYOUT_TACTVISOR_SIZE_X - 1), (oh_output_coord_t) (BH_LAYOUT_TACTVISOR_SIZE_Y - 1))

#define BH_LAYOUT_TACTVISOR_SIZE (BH_LAYOUT_TACTVISOR_SIZE_X * BH_LAYOUT_TACTVISOR_SIZE_Y)
#define BH_LAYOUT_TACTVISOR {           \
  BH_LAYOUT_TACTVISOR_MAKE_POINT(0, 0), \
  BH_LAYOUT_TACTVISOR_MAKE_POINT(1, 0), \
  BH_LAYOUT_TACTVISOR_MAKE_POINT(2, 0), \
  BH_LAYOUT_TACTVISOR_MAKE_POINT(3, 0), \
}

#pragma endregion BH_DEVICE_TACTVISOR

#pragma region BH_DEVICE_TACTOSY2

#define BH_LAYOUT_TACTOSY2_SIZE_X 3
#define BH_LAYOUT_TACTOSY2_SIZE_Y 2
#define BH_LAYOUT_TACTOSY2_MAKE_POINT(x, y) OH::PlaneMapper_Margin::mapPoint<oh_output_coord_t>(x, y, (oh_output_coord_t) (BH_LAYOUT_TACTOSY2_SIZE_X - 1), (oh_output_coord_t) (BH_LAYOUT_TACTOSY2_SIZE_Y - 1))

#define BH_LAYOUT_TACTOSY2_SIZE (BH_LAYOUT_TACTOSY2_SIZE_X * BH_LAYOUT_TACTOSY2_SIZE_Y)
#define BH_LAYOUT_TACTOSY2 {                                                                                       \
    BH_LAYOUT_TACTOSY2_MAKE_POINT(0, 0), BH_LAYOUT_TACTOSY2_MAKE_POINT(1, 0), BH_LAYOUT_TACTOSY2_MAKE_POINT(2, 0), \
    BH_LAYOUT_TACTOSY2_MAKE_POINT(0, 1), BH_LAYOUT_TACTOSY2_MAKE_POINT(1, 1), BH_LAYOUT_TACTOSY2_MAKE_POINT(2, 1), \
}

#pragma endregion BH_DEVICE_TACTOSY2

#pragma region BH_DEVICE_TACTOSYH

#define BH_LAYOUT_TACTOSYH_SIZE_X 1
#define BH_LAYOUT_TACTOSYH_SIZE_Y 3
#define BH_LAYOUT_TACTOSYH_MAKE_POINT(x, y) OH::PlaneMapper_Margin::mapPoint<oh_output_coord_t>(x, y, (oh_output_coord_t) (BH_LAYOUT_TACTOSYH_SIZE_X - 1), (oh_output_coord_t) (BH_LAYOUT_TACTOSYH_SIZE_Y - 1))

#define BH_LAYOUT_TACTOSYH_SIZE (BH_LAYOUT_TACTOSYH_SIZE_X * BH_LAYOUT_TACTOSYH_SIZE_Y)
#define BH_LAYOUT_TACTOSYH { BH_LAYOUT_TACTOSYH_MAKE_POINT(0, 0), BH_LAYOUT_TACTOSYH_MAKE_POINT(0, 1), BH_LAYOUT_TACTOSYH_MAKE_POINT(0, 2) }

#pragma endregion BH_DEVICE_TACTOSYH

#pragma region BH_DEVICE_TACTOSYF

#define BH_LAYOUT_TACTOSYF_SIZE_X 1
#define BH_LAYOUT_TACTOSYF_SIZE_Y 3
#define BH_LAYOUT_TACTOSYF_MAKE_POINT(x, y) OH::PlaneMapper_Margin::mapPoint<oh_output_coord_t>(x, y, (oh_output_coord_t) (BH_LAYOUT_TACTOSYF_SIZE_X - 1), (oh_output_coord_t) (BH_LAYOUT_TACTOSYF_SIZE_Y - 1))

#define BH_LAYOUT_TACTOSYF_SIZE (BH_LAYOUT_TACTOSYF_SIZE_X * BH_LAYOUT_TACTOSYF_SIZE_Y)
#define BH_LAYOUT_TACTOSYF { BH_LAYOUT_TACTOSYF_MAKE_POINT(0, 0), BH_LAYOUT_TACTOSYF_MAKE_POINT(0, 1), BH_LAYOUT_TACTOSYF_MAKE_POINT(0, 2), }

#pragma endregion BH_DEVICE_TACTOSYF

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

#define BH_BLE_SERVICE_DFU_UUID \
  BLEUUID("0000fe59-0000-1000-8000-00805f9b34fb")
#define BH_BLE_SERVICE_DFU_CHAR_CONTROL_UUID \
  BLEUUID("8ec90003-f315-4f60-9fb8-838830daea50")
