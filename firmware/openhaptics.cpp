#include "config/all.h"

#include "openhaptics.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif

#include <logging.hpp>

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

  this->components.insert(component);
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

void OpenHaptics::postEvent(const OH::IEvent* event) {
  log_i("Even dispatched at %u: %s (%p)", millis(), event->eventName.c_str(), event);

  for (auto* listener : this->eventListeners) {
    listener->handleEvent(event);
  }

  delete event;
}

void OpenHaptics::addEventListener(const OH::IEventListener* listener) {
  this->eventListeners.push_back(listener);
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
