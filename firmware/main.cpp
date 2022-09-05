#include "firmware.h"
#include "main.h"

#ifdef SERIAL_PLOTTER
#include "components/serial_plotter.h"
#endif

extern void setupMode(void);

void setup()
{
    Serial.begin(115200);
    setupMode();

    #ifdef SERIAL_PLOTTER
    SerialPlotter_OutputStates* serialOutputState = new SerialPlotter_OutputStates(Serial);
    App.registerComponent(serialOutputState);
    #endif

    App.setup();
}

void loop()
{
    App.loop();
}
