#pragma once

#ifndef BLUETOOTH_NAME
#define BLUETOOTH_NAME "SenseShift"
#endif

#ifndef BLUETOOTH_ADDRESS
#define BLUETOOTH_ADDRESS                  \
    {                                      \
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB \
    }
#endif

#ifndef SS_BLE_USE_NIMBLE
#define SS_BLE_USE_NIMBLE false
#endif
