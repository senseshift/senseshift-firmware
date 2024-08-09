#pragma once

#include "config/all.h"

#include "senseshift/battery/input/battery_sensor.hpp"
#include <senseshift/body/haptics/body.hpp>
#include <senseshift/events.hpp>

#include <vector>

namespace SenseShift {
class Application final : public IEventDispatcher {
  public:
    Application()
    {
        this->vibro_body_ = new Body::Haptics::FloatBody();
    }

    ~Application()
    {
        delete this->vibro_body_;
    }

    Application(const Application& other)
    {
        this->vibro_body_ = new Body::Haptics::FloatBody(*other.vibro_body_);
        this->event_listeners_ = other.event_listeners_;
    }

    Application(Application&& other) noexcept :
      vibro_body_(other.vibro_body_), event_listeners_(std::move(other.event_listeners_))
    {
        other.vibro_body_ = nullptr;
    }

    Application& operator=(const Application& other)
    {
        if (this == &other)
            return *this; // handle self-assignment

        delete this->vibro_body_;

        this->vibro_body_ = new Body::Haptics::FloatBody(*other.vibro_body_);
        this->event_listeners_ = other.event_listeners_;

        return *this;
    }

    Application& operator=(Application&& other) noexcept
    {
        if (this == &other)
            return *this; // handle self-assignment

        delete this->vibro_body_;

        this->vibro_body_ = other.vibro_body_;
        event_listeners_ = std::move(other.event_listeners_);

        other.vibro_body_ = nullptr;

        return *this;
    }

    auto getVibroBody() const -> Body::Haptics::FloatBody*
    {
        return this->vibro_body_;
    }

    void postEvent(const IEvent* event) override;
    void addEventListener(const IEventListener* listener) override;

  private:
    Body::Haptics::FloatBody* vibro_body_;
    std::vector<const IEventListener*> event_listeners_{};
};
} // namespace SenseShift
