#pragma once

#include <senseshift/events.hpp>

#include <cstdint>

namespace SenseShift::Battery {
    struct BatteryState {
        uint8_t level;
    };

    class BatteryLevelEvent : public IEvent {
      public:
        const BatteryState& state;
        BatteryLevelEvent(const BatteryState& state) : IEvent(OH_EVENT_BATTERY_LEVEL), state(state){};
    };
} // namespace SenseShift::Battery
