#include "config/all.h"

#include "senseshift.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif

#include <senseshift/core/logging.hpp>

namespace SenseShift {
    static const char* const TAG = "application";

    Application::Application()
    {
        this->vibro_body_ = new Body::Haptics::FloatBody();
    }

    void Application::postEvent(const IEvent* event)
    {
        LOG_I(TAG, "Event dispatched at %u: %s (%p)", millis(), event->eventName.c_str(), event);

        for (const auto* listener : this->event_listeners_) {
            listener->handleEvent(event);
        }

        delete event;
    }

    void Application::addEventListener(const IEventListener* listener)
    {
        this->event_listeners_.push_back(listener);
    }
} // namespace SenseShift
