#pragma once

#include "output.h"
#include <abstract_output_writer.hpp>

#include <Arduino.h>

template <typename _Tp>
oh_output_point_t* getPoint(_Tp x, _Tp y, _Tp x_max, _Tp y_max) {
  const oh_output_coord_t x_coord = map(x, 0, x_max, 0, OH_OUTPUT_COORD_MAX);
  const oh_output_coord_t y_coord = map(y, 0, y_max, 0, OH_OUTPUT_COORD_MAX);

  return new oh_output_point_t(x_coord, y_coord);
}

oh_output_writers_map_t transformAutoOutput(std::vector<std::vector<OH::AbstractOutputWriter*>> map2d);
