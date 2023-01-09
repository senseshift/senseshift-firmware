#include "auto_output.h"

outputPoint_t* getPoint(uint16_t x, uint16_t y, uint16_t x_max, uint16_t y_max) {
  const uint16_t x_coord =
      1 == x_max ? 1 : UINT16_MAX * (1 / ((float)x_max - 1)) * ((float)x);
  const uint16_t y_coord =
      1 == y_max ? 1 : UINT16_MAX * (1 / ((float)y_max - 1)) * ((float)y);

  return new outputPoint_t(x_coord, y_coord);
}

outputMap_t transformAutoOutput(std::vector<std::vector<OutputWriter*>> map2d) {
  outputMap_t points{};

  size_t y_max = map2d.size();

  for (size_t y = 0; y < y_max; ++y) {
    auto row = map2d.at(y);
    size_t x_max = row.size();

    for (size_t x = 0; x < x_max; ++x) {
      OutputWriter* wr = row.at(x);
      outputPoint_t* coord = getPoint(x, y, x_max, y_max);

      points[*coord] = wr;
    }
  }

  return points;
}
