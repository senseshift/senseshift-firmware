#include "auto_output.h"

#include <logging.hpp>

oh_output_writers_map_t transformAutoOutput(
    std::vector<std::vector<OH::AbstractOutputWriter*>> map2d) {
  oh_output_writers_map_t points{};

  size_t y_size = map2d.size();

  for (size_t y = 0; y < y_size; ++y) {
    auto row = map2d.at(y);
    size_t x_size = row.size();

    for (size_t x = 0; x < x_size; ++x) {
      OH::AbstractOutputWriter* wr = row.at(x);
      oh_output_point_t* coord = getPoint(x, y, x_size - 1, y_size - 1);

      log_v("map2d[%u][%u] = (%u/%u, %u/%u) = (%3u, %3u)\n", y, x, x, x_size - 1, y, y_size - 1, coord->x, coord->y);

      points[*coord] = wr;
    }
  }

  return points;
}
