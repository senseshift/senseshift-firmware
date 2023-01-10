#include <math.h>
#include <algorithm>

#include "output_components/closest.h"

oh_output_point_t ClosestOutputComponent::findClosestPoints(
    std::list<oh_output_point_t>& pts,
    oh_output_point_t& target) {
  std::multimap<float, oh_output_point_t> mp = {};

  for (auto& _p : pts) {
    if (target == _p) {
      return _p;  // if coord == target, no other needed
    }

    float dx = abs(((float)target.x / UINT16_MAX) - ((float)_p.x / UINT16_MAX)),
          dy = abs(((float)target.y / UINT16_MAX) - ((float)_p.y / UINT16_MAX));

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

void ClosestOutputComponent::writeOutput(oh_output_data_t& data) {
  auto closestPoint = this->findClosestPoints(this->points, data.point);

  auto state = &this->states[closestPoint];
  state->intensity = data.intensity;

  this->writers.at(closestPoint)->writeOutput(state->intensity);
}
