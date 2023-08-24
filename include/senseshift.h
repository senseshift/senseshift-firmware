#pragma once

#include "config/all.h"

#include <haptic_body.hpp>
#include <senseshift/battery/sensor.hpp>
#include <senseshift/events.hpp>

#include <vector>

namespace SenseShift {
    class SenseShift final : public IEventDispatcher {
      private:
        std::vector<const IEventListener*> eventListeners{};
        Body::Haptics::HapticBody* pHapticBody;
        ::SenseShift::Battery::IBatterySensor* battery;

      public:
        SenseShift();

        Body::Haptics::HapticBody* getHapticBody() { return this->pHapticBody; };

        void postEvent(const IEvent* event) override;
        void addEventListener(const IEventListener* listener) override;
    };
} // namespace SenseShift
