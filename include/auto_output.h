#pragma once

#include "output.h"

Point2D* getPoint(uint16_t x, uint16_t y, uint16_t x_max, uint16_t y_max);

outputMap_t transformAutoOutput(std::vector<std::vector<OutputWriter*>> map2d);
