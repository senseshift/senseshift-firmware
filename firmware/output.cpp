#include "output.h"
#include "openhaptics.h"

Output::Output() {}

void Output::addComponent(oh_output_path_t path, OH::OutputComponent* c) {
  this->components[path] = c;
  App.registerComponent(c);
}

std::map<oh_output_path_t, OH::OutputComponent*>* Output::getComponents() {
  return &this->components;
}

void Output::writeOutput(oh_output_path_t path, oh_output_data_t& data) {
  auto componentSearch = this->getComponents()->find(path);

  if (componentSearch == this->getComponents()->end()) {
    // if no requested component exists, skip
    return;
  }

  (*componentSearch).second->writeOutput(data);
}
