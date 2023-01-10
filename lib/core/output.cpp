#include "output.hpp"

void OH::Output::addComponent(oh_output_path_t path, OH::OutputComponent* c) {
  this->components[path] = c;
  this->app->registerComponent(c);
}

std::map<oh_output_path_t, OH::OutputComponent*>* OH::Output::getComponents() {
  return &this->components;
}

void OH::Output::writeOutput(oh_output_path_t path, oh_output_data_t& data) {
  auto componentSearch = this->getComponents()->find(path);

  if (componentSearch == this->getComponents()->end()) {
    // if no requested component exists, skip
    return;
  }

  (*componentSearch).second->writeOutput(data);
}
