#include "config/all.h"

#include "openhaptics.h"

OpenHaptics::OpenHaptics() {
  this->output = new OH::Output(this);
}

void OpenHaptics::registerComponent(OH::AbstractComponent* component) {
  if (component == nullptr) {
    return;
  }

  for (auto* c : this->components) {
    if (c == component) {
      return;
    }
  }

  this->components.push_back(component);
}

void OpenHaptics::addOutputComponent(oh_output_path_t path,
                                     OH::OutputComponent* c) {
  this->registerComponent(c);
  this->getOutput()->addComponent(path, c);
}

void OpenHaptics::setConnection(OH::AbstractConnection* connection) {
  this->registerComponent(connection);
  this->connection = connection;
}

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
void OpenHaptics::setBattery(OH::AbstractBattery* battery) {
  this->registerComponent(battery);
  this->battery = battery;
}
#endif

void OpenHaptics::setup() {
  for (auto& component : this->components) {
    component->setup();
  }
}

void OpenHaptics::loop() {
  for (auto* c : this->components) {
    c->loop();
  }
}

OpenHaptics App;
