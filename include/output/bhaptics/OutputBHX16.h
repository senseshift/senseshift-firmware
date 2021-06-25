#pragma once

#include <stdint.h>

#include "config/bhaptics.h"
#include "output/Output.h"

class OutputBHX16 : public Output
{
    private:
        const uint8_t Motors[16] = {
            O_V_X16_M0,  O_V_X16_M4,  O_V_X16_M8, O_V_X16_M12,
            O_V_X16_M1,  O_V_X16_M5,  O_V_X16_M9, O_V_X16_M13,
            O_V_X16_M2, O_V_X16_M6, O_V_X16_M10, O_V_X16_M14,
            O_V_X16_M3, O_V_X16_M7, O_V_X16_M11, O_V_X16_M15
        };

    public:
        void setup();
        void writeMotors(uint8_t[]);
        void writeMotor(uint8_t index, float intensity);
};

