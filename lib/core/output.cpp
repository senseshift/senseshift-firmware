#include "output.hpp"

#include <logging.hpp>

void OH::Output::addComponent(oh_output_path_t path, OH::OutputComponent* c) {
  this->components[path] = c;
  this->app->registerComponent(c);
}

std::map<oh_output_path_t, OH::OutputComponent*>* OH::Output::getComponents() {
  return &this->components;
}

void OH::Output::writeOutput(oh_output_path_t path, oh_output_data_t& data) {
  if (this->getComponents()->count(path) == 0) {
    // if no requested component exists, skip
    log_e("No component found for path %d", path);
    return;
  }

  auto componentSearch = this->getComponents()->find(path);
  (*componentSearch).second->writeOutput(data);
}