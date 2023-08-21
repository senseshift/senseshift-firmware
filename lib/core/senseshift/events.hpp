#pragma once

#include <string>

#define OH_EVENT_BATTERY_LEVEL "battery_level"
#define OH_EVENT_CONNECTED "connected"
#define OH_EVENT_DISCONNECTED "disconnected"

namespace SenseShift {
    enum Event {

    };

    struct IEvent {
        const std::string eventName;

        IEvent(const std::string eventName) : eventName(eventName){};
    };

    class IEventListener {
      public:
        virtual void handleEvent(const IEvent* event) const = 0;
    };

    class IEventDispatcher {
      public:
        virtual void postEvent(const IEvent* event) = 0;
        virtual void addEventListener(const IEventListener* handler) = 0;
    };
} // namespace SenseShift
