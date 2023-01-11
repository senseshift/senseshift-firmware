#pragma once

#include <abstract_output_writer.hpp>
#include <output.hpp>

#include <Arduino.h>

template <typename _Tp>
oh_output_point_t* getPoint(_Tp x, _Tp y, _Tp x_max, _Tp y_max) {
  const oh_output_coord_t x_coord =
      1 == x_max ? 1 : OH_OUTPUT_COORD_MAX * (1 / (float)x_max) * ((float)x);
  const oh_output_coord_t y_coord =
      1 == y_max ? 1 : OH_OUTPUT_COORD_MAX * (1 / (float)y_max) * ((float)y);

  return new oh_output_point_t(x_coord, y_coord);
}

oh_output_writers_map_t transformAutoOutput(
    std::vector<std::vector<OH::AbstractOutputWriter*>> map2d);
