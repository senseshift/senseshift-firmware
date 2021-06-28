#include "firmware.h"
#include "main.h"

extern void setupMode(void);

void setup()
{
    setupMode();

    App.setup();
}

void loop()
{
    App.loop();
}
