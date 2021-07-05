#pragma once

#include <vector>

#include "output.h"

typedef std::vector<std::vector<OutputWriter*>> autoOutputVector_t;

class OutputAutoComponent : public OutputComponent
{
    protected:
        virtual uint16_t getCoordinateX(size_t x, size_t x_max);
        virtual uint16_t getCoordinateY(size_t y, size_t y_max);
        virtual outputPoint_t getCoordinate(size_t x, size_t y, size_t x_max, size_t y_max);

    public:
        OutputAutoComponent(autoOutputVector_t&);
};
