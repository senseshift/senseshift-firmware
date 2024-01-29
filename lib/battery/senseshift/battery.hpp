#pragma once

#include <cstdint>
#include <limits>
#include <algorithm>

#include <frozen/map.h>

#include <senseshift/events.hpp>

namespace SenseShift::Battery {

    namespace VoltageMap {
        /// Lookup table for LiPO 1S 4.2V batteries
        /// \see <a href="https://blog.ampow.com/lipo-voltage-chart/">Source</a>
        [[maybe_unused]] inline static const auto LiPO_1S_42= frozen::make_map<float, float>({
            { 4.2, 1.0 },
            { 4.15, 0.95 },
            { 4.11, 0.9 },
            { 4.08, 0.85 },
            { 4.02, 0.8 },
            { 3.98, 0.75 },
            { 3.95, 0.7 },
            { 3.91, 0.65 },
            { 3.87, 0.6 },
            { 3.85, 0.55 },
            { 3.84, 0.5 },
            { 3.82, 0.45 },
            { 3.8, 0.4 },
            { 3.79, 0.35 },
            { 3.77, 0.3 },
            { 3.75, 0.25 },
            { 3.73, 0.2 },
            { 3.71, 0.15 },
            { 3.69, 0.1 },
            { 3.61, 0.05 },
            { 3.27, 0.0 },
        });
    } // namespace VoltageMap

    struct BatteryState {
        using VoltageType = float;
        using LevelType = std::uint8_t;

        static constexpr LevelType MAX_LEVEL = std::numeric_limits<LevelType>::max();
        LevelType level;
    };

    class BatteryLevelEvent : public IEvent {
      public:
        const BatteryState& state;
        BatteryLevelEvent(const BatteryState& state) : IEvent(OH_EVENT_BATTERY_LEVEL), state(state){};
    };
} // namespace SenseShift::Battery
