#pragma once

#include "output.h"
#include <abstract_output_writer.hpp>

outputPoint_t* getPoint(uint16_t x, uint16_t y, uint16_t x_max, uint16_t y_max);

outputMap_t transformAutoOutput(std::vector<std::vector<OH::AbstractOutputWriter*>> map2d);
