#include "config/all.h"

#include "openhaptics.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif

#include <logging.hpp>

OpenHaptics::OpenHaptics() {
  this->pHapticBody = new OH::HapticBody();
}

void OpenHaptics::postEvent(const OH::IEvent* event) {
  log_i("Event dispatched at %u: %s (%p)", millis(), event->eventName.c_str(), event);

  for (auto* listener : this->eventListeners) {
    listener->handleEvent(event);
  }

  delete event;
}

void OpenHaptics::addEventListener(const OH::IEventListener* listener) {
  this->eventListeners.push_back(listener);
}
