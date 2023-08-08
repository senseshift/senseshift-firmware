#pragma once

#include "config/all.h"

#include <senseshift_interface.hpp>

#include <abstract_connection.hpp>
#include <abstract_battery.hpp>

#include <events.hpp>
#include <haptic_body.hpp>

#include <optional>
#include <vector>

class SenseShiftApp final : public OH::IEventDispatcher {
  private:
    std::vector<const OH::IEventListener*> eventListeners{};
    OH::HapticBody* pHapticBody;
    std::optional<OH::BatterySensor*> batterySensor;

  public:
    SenseShiftApp();

    OH::HapticBody* getHapticBody()
    {
        return this->pHapticBody;
    };

    void postEvent(const OH::IEvent* event) override;
    void addEventListener(const OH::IEventListener* listener) override;

    void handleMessage(const SenseShift::Interface::Message& message);
};
