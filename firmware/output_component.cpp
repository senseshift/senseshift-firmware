#include "output_component.h"

void OutputComponent::setOutputs(outputMap_t& outputs) {
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

void OutputComponent::setup() {
  for (const auto& kv : this->writers) {
    kv.second->setup();
  }
}

void OutputComponent::loop() {
  for (const auto& kv : this->writers) {
    kv.second->loop();
  }
}
