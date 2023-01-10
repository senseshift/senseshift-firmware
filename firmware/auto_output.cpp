#include "auto_output.h"

oh_output_writers_map_t transformAutoOutput(
    std::vector<std::vector<OH::AbstractOutputWriter*>> map2d) {
  oh_output_writers_map_t points{};

  size_t y_max = map2d.size();

  for (size_t y = 0; y < y_max; ++y) {
    auto row = map2d.at(y);
    size_t x_max = row.size();

    for (size_t x = 0; x < x_max; ++x) {
      OH::AbstractOutputWriter* wr = row.at(x);
      oh_output_point_t* coord = getPoint(x, y, x_max, y_max);

      points[*coord] = wr;
    }
  }

  return points;
}
