#pragma once

#include <HardwareSerial.h>

#include "component.h"

class SerialPlotter_OutputStates : public Component
{
    protected:
        HardwareSerial* serial;
    public:
        SerialPlotter_OutputStates(HardwareSerial &serial) : serial(&serial) {};
        void setup(void);
        void loop(void);
};
