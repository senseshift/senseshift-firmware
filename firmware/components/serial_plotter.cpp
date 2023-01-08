#include "components/serial_plotter.h"
#include "openhaptics.h"

void SerialPlotter_OutputStates::setup() {
  this->serial->begin(115200);
}

void SerialPlotter_OutputStates::loop() {
  for (auto& _c : *App.getOutput()->getComponents()) {
    for (auto& _s : *_c.second->getOutputStates()) {
      this->serial->printf("%5u \t", _s.second.intensity);
    }
  }
  this->serial->println();
}
