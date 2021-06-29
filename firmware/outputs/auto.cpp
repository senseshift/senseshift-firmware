#include <Arduino.h>

#include "outputs/auto.h"

OutputAutoComponent::OutputAutoComponent(autoOutputVector_t &outputs)
{
    std::map<outputPoint_t*, OutputWriter*> points{};

    size_t y_max = outputs.size();

    for (size_t y = 0; y < y_max; ++y)
    {
        auto row = outputs.at(y);
        size_t x_max = row.size();

        for (size_t x = 0; x < x_max; ++x)
        {
            OutputWriter* wr = row.at(x);
            Point2D coord = this->getCoordinate(x, y, x_max, y_max);

            points[&coord] = wr;
        }
    }

    OutputComponent::setOutputs(points);
}

uint16_t OutputAutoComponent::getCoordinateX(size_t x, size_t x_max)
{
    return map(x, 0, x_max, 0, UINT16_MAX);
}

uint16_t OutputAutoComponent::getCoordinateY(size_t y, size_t y_max)
{
    return map(y, 0, y_max, 0, UINT16_MAX);
}

outputPoint_t OutputAutoComponent::getCoordinate(size_t x, size_t y, size_t x_max, size_t y_max)
{
    outputPoint_t point;

    point.x = this->getCoordinateX(x, x_max);
    point.y = this->getCoordinateY(y, y_max);

    return point;
}