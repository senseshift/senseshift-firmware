#include "output_component.hpp"

#include <logging.hpp>

void OH::OutputComponent::setOutputs(oh_output_writers_map_t& outputs) {
  this->writers.clear();
  this->writers = outputs;

  this->points.clear();
  for (auto& _p : outputs) {
    this->points.push_back(_p.first);
  }

  this->states.clear();
  for (auto& _p : outputs) {
    this->states[_p.first] = {};
  }
}

void OH::OutputComponent::setup() {
  for (const auto& kv : this->writers) {
    kv.second->setup();
  }
}

void OH::OutputComponent::writeOutput(const oh_output_data_t& data) {
  if (this->writers.count(data.point) == 0) {
    log_w("No writer for point (%u, %u)", data.point.x, data.point.y);
    return;
  }

  this->writers[data.point]->writeOutput(data.intensity);
}

oh_output_point_t OH::ClosestOutputComponent::findClosestPoints(
    std::list<oh_output_point_t>& pts,
    const oh_output_point_t& target) {
  std::multimap<float, oh_output_point_t> mp = {};

  for (auto& _p : pts) {
    if (target == _p) {
      return _p;  // if coord == target, no other needed
    }

    float dx = abs(((float)target.x / OH_OUTPUT_COORD_MAX) - ((float)_p.x / OH_OUTPUT_COORD_MAX)),
          dy = abs(((float)target.y / OH_OUTPUT_COORD_MAX) - ((float)_p.y / OH_OUTPUT_COORD_MAX));

    auto dist = (float)sqrt(pow(dx, 2) + pow(dy, 2));

    mp.insert({dist, _p});
  }

  auto nearest =
      std::min_element(mp.begin(), mp.end(),
                       [](const std::pair<float, oh_output_point_t>& a,
                          const std::pair<float, oh_output_point_t>& b) {
                         return a.first < b.first;
                       });

  return nearest->second;
}

void OH::ClosestOutputComponent::writeOutput(const oh_output_data_t& data) {
  auto closestPoint = this->findClosestPoints(this->points, data.point);

  auto state = &this->states[closestPoint];
  state->intensity = data.intensity;

  this->writers.at(closestPoint)->writeOutput(state->intensity);
}
