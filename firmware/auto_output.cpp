#include "auto_output.h"

#include <logging.hpp>

oh_output_writers_map_t transformAutoOutput(std::vector<std::vector<OH::AbstractOutputWriter*>> map2d) {
  oh_output_writers_map_t points{};

  size_t y_size = map2d.size();
  size_t y_max = y_size - 1;

  for (size_t y = 0; y < y_size; ++y) {
    auto row = map2d.at(y);
    size_t x_size = row.size();
    size_t x_max = x_size - 1;

    for (size_t x = 0; x < x_size; ++x) {
      OH::AbstractOutputWriter* wr = row.at(x);
      oh_output_point_t* coord = getPoint(x, y, x_max, y_max);

      log_v("map2d[%u][%u] = (%u/%u, %u/%u) = (%3u, %3u)", y, x, x, x_max, y, y_max, coord->x, coord->y);

      points[*coord] = wr;
    }
  }

  return points;
}
