#pragma once

#include <senseshift/events.hpp>

#include <cstdint>

namespace SenseShift::Battery {
    typedef struct BatteryState {
        uint8_t level;
    } BatteryState_t;

    class BatteryLevelEvent : public IEvent {
      public:
        const BatteryState_t& state;
        BatteryLevelEvent(const BatteryState_t& state) : IEvent(OH_EVENT_BATTERY_LEVEL), state(state){};
    };
} // namespace SenseShift::Battery
