#pragma once

#include "config/all.h"

#include <senseshift/battery/sensor.hpp>
#include <senseshift/body/haptics/body.hpp>
#include <senseshift/events.hpp>

#include <vector>

namespace SenseShift {
    class Application final : public IEventDispatcher {
      public:
        Application();

        [[nodiscard]] auto getVibroBody() const -> Body::Haptics::FloatBody* { return this->vibro_body_; };

        void postEvent(const IEvent* event) override;
        void addEventListener(const IEventListener* listener) override;

      private:
        std::vector<const IEventListener*> event_listeners_{};
        Body::Haptics::FloatBody* vibro_body_;
        Battery::IBatterySensor* battery_ = nullptr;
    };
} // namespace SenseShift
