#include "firmware.h"
#include "main.h"

#ifdef CONNECTION_BHAPTICS_BLE
#include "connections/bhaptics.h"
#endif

void setup()
{
    #ifdef CONNECTION_BHAPTICS_BLE
    BHapticsBLEConnection* bhBleConnection = new BHapticsBLEConnection(BLUETOOTH_NAME);
    App.registerComponent(bhBleConnection);
    #endif

    App.setup();
}

void loop()
{
    App.loop();
}
