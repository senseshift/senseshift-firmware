#include "config/all.h"

#include "senseshift.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif

#include <logging.hpp>

SenseShiftCore::SenseShiftCore()
{
    this->pHapticBody = new OH::HapticBody();
}

void SenseShiftCore::postEvent(const OH::IEvent* event)
{
    log_i("Event dispatched at %u: %s (%p)", millis(), event->eventName.c_str(), event);

    for (auto* listener : this->eventListeners) {
        listener->handleEvent(event);
    }

    delete event;
}

void SenseShiftCore::addEventListener(const OH::IEventListener* listener)
{
    this->eventListeners.push_back(listener);
}
