#pragma once

#include "outputs/auto.h"

class OutputAutoComponent_Margin : public OutputAutoComponent
{
    protected:
        uint16_t getCoordinateX(size_t x, size_t x_max);
        uint16_t getCoordinateY(size_t y, size_t y_max);

    public:
        OutputAutoComponent_Margin(autoOutputVector_t &outputs): OutputAutoComponent(outputs) {};
};
