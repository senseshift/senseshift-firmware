#include "config/all.h"

#include "senseshift.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif

#include <logging.hpp>

SenseShift::SenseShift()
{
    this->pHapticBody = new OH::HapticBody();
}

void SenseShift::postEvent(const OH::IEvent* event)
{
    log_i("Event dispatched at %u: %s (%p)", millis(), event->eventName.c_str(), event);

    for (auto* listener : this->eventListeners) {
        listener->handleEvent(event);
    }

    delete event;
}

void SenseShift::addEventListener(const OH::IEventListener* listener)
{
    this->eventListeners.push_back(listener);
}
