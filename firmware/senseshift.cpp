#include "config/all.h"

#include "senseshift.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif

#include <senseshift/logging.hpp>

namespace SenseShift {
    SenseShift::SenseShift()
    {
        this->pHapticBody = new Body::Haptics::HapticBody();
    }

    void SenseShift::postEvent(const IEvent* event)
    {
        log_i("Event dispatched at %u: %s (%p)", millis(), event->eventName.c_str(), event);

        for (auto* listener : this->eventListeners) {
            listener->handleEvent(event);
        }

        delete event;
    }

    void SenseShift::addEventListener(const IEventListener* listener)
    {
        this->eventListeners.push_back(listener);
    }
} // namespace SenseShift
