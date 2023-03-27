#include "output.hpp"

#include <logging.hpp>

void OH::Output::addComponent(OH::OutputComponent* c) {
  auto path = c->getPath();
  this->components[path] = c;
}

std::map<oh_output_path_t, OH::OutputComponent*>* OH::Output::getComponents() {
  return &this->components;
}

void OH::Output::writeOutput(const oh_output_path_t path, const oh_output_data_t& data) {
  if (this->getComponents()->count(path) == 0) {
    // if no requested component exists, skip
    log_w("No component found for path %d", path);
    return;
  }

  auto componentSearch = this->getComponents()->find(path);
  (*componentSearch).second->writeOutput(data);
}

template<typename _Tp>
inline std::map<oh_output_point_t, _Tp*> OH::mapMatrixCoordinates(std::vector<std::vector<_Tp*>> map2d) {
  std::map<oh_output_point_t, _Tp*> points{};

  size_t y_size = map2d.size();
  size_t y_max = y_size - 1;

  for (size_t y = 0; y < y_size; ++y) {
    auto row = map2d.at(y);
    size_t x_size = row.size();
    size_t x_max = x_size - 1;

    for (size_t x = 0; x < x_size; ++x) {
      AbstractOutputWriter* wr = row.at(x);
      oh_output_point_t* coord = mapPoint(x, y, x_max, y_max);

      points[*coord] = wr;
    }
  }

  return points;
};

template oh_output_writers_map_t OH::mapMatrixCoordinates<OH::AbstractOutputWriter>(std::vector<std::vector<OH::AbstractOutputWriter*>>);
