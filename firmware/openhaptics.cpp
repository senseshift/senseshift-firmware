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

void OpenHaptics::addOutputComponent(OH::OutputComponent* c) {
  this->registerComponent(c);
  this->getOutput()->addComponent(c->getPath(), c);
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

void OpenHaptics::begin(void) {
  for (auto& c : this->components) {
    c->setup();
  }

  for (auto& c : this->components) {
    c->begin();
  }
}

OpenHaptics App;
