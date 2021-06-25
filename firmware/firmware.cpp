#include <Arduino.h>
#include <stdint.h>

#include "config.h"

#if OH_CONNECTION == CONNECTION_BLE
#include "connection/bhaptics/ConnectionBH.h"
Connection *conn = new ConnectionBH(BLUETOOTH_NAME);
#endif

#if OH_OUTPUT == OUTPUT_VEST_X16
#include "output/bhaptics/OutputBHX16.h"
Output *output = new OutputBHX16();
#endif

void setup()
{
    conn->start();
}

void loop()
{
    if (conn->isOpen())
    {
        // conn->loop(lang);
    }
}
