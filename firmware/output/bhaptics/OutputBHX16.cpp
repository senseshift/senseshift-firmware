#include "output/bhaptics/OutputBHX16.h"

#include <Arduino.h>

void OutputBHX16::setup()
{
    ;
}

void OutputBHX16::writeMotors(uint8_t motors[])
{
    uint8_t mappedMotors[16] = { 0 };

    for (uint8_t i = 0; i < 16; i++)
    {
        uint8_t actIndexStart = i * 2 + (i * 2 / 8) * 2;

        if ((i + 1) % 4 == 0) { // bottom 4
            this->writeMotor(i, max(max(motors[actIndexStart], motors[actIndexStart+1]), max(motors[actIndexStart+2], motors[actIndexStart+3])));
        } else {
            this->writeMotor(i, max(motors[actIndexStart], motors[actIndexStart+1]));
        }
    }
}

void OutputBHX16::writeMotor(uint8_t index, float intensity)
{
    
}