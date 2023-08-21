#pragma once

#include "config/all.h"

#include <haptic_body.hpp>
#include <senseshift/events.hpp>

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include <abstract_battery.hpp>
#endif

#include <vector>

namespace SenseShift {
    class SenseShift final : public IEventDispatcher {
      private:
        std::vector<const IEventListener*> eventListeners{};
        Body::Haptics::HapticBody* pHapticBody;

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
        OH::BatterySensor* battery;
#endif

      public:
        SenseShift();

        Body::Haptics::HapticBody* getHapticBody()
        {
            return this->pHapticBody;
        };

        void postEvent(const IEvent* event) override;
        void addEventListener(const IEventListener* listener) override;
    };
} // namespace SenseShift
