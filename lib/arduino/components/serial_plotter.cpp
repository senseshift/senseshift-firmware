#include "components/serial_plotter.hpp"

template<typename _Tp>
void OH::SerialPlotter_OutputStates<_Tp>::run() {
  while (true) {
    for (auto& _c : *output->getComponents()) {
      oh_output_path_t path = _c.first;
      OH::HapticPlane* component = _c.second;

      for (auto& _s : *component->getOutputStates()) {
        oh_output_point_t point = _s.first;
        oh_output_state_t state = _s.second;

        this->serial->printf("Output[%u][%ux%u]:%u, ", path, point.x, point.y, state.intensity);
      }
    }
    this->serial->println();

    delay(this->sampleRate);
  }
}
