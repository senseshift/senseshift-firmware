#include "output_component.hpp"

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

void OH::OutputComponent::writeOutput(oh_output_data_t& data) {
  this->writers[data.point]->writeOutput(data.intensity);
}
