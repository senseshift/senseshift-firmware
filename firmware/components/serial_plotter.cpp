#include "components/serial_plotter.h"
#include "openhaptics.h"

template<typename _Tp>
void SerialPlotter_OutputStates<_Tp>::loop() {
  for (auto& _c : *App.getOutput()->getComponents()) {
    oh_output_path_t path = _c.first;
    OH::OutputComponent* component = _c.second;

    for (auto& _s : *component->getOutputStates()) {
      oh_output_point_t point = _s.first;
      oh_output_state_t state = _s.second;

      this->serial->printf("Output[%u][%ux%u]:%u, ", path, point.x, point.y, state.intensity);
    }
  }
  this->serial->println();

  delay(this->sampleRate);
}
