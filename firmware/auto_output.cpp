#include "auto_output.h"

inline Point2D* getPoint(uint16_t x, uint16_t y, uint16_t x_max, uint16_t y_max)
{
    return new Point2D(UINT16_MAX * (1 / ((float)x_max - 1)) * ((float)x), UINT16_MAX * (1 / ((float)y_max - 1)) * ((float)y));
}

outputMap_t transformAutoOutput(std::vector<std::vector<OutputWriter*>> map2d)
{
    outputMap_t points{};

    size_t y_max = map2d.size();

    for (size_t y = 0; y < y_max; ++y)
    {
        auto row = map2d.at(y);
        size_t x_max = row.size();

        for (size_t x = 0; x < x_max; ++x)
        {
            OutputWriter* wr = row.at(x);
            Point2D* coord = getPoint(x, y, x_max, y_max);

            points[*coord] = wr;
        }
    }

    return points;
}