#pragma once

#include <cstdint>

#include <frozen/map.h>

#include <senseshift/events.hpp>

namespace SenseShift::Battery {
    // Source: https://blog.ampow.com/lipo-voltage-chart/
    inline static const auto voltageToPercents_1s = frozen::make_map<float, float>({
      // clang-format off
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
      // clang-format on
    });
    inline static const auto voltageToPercents_2s = frozen::make_map<float, float>({
      // clang-format off
        { 8.4, 1.0 },
        { 8.3, 0.95 },
        { 8.22, 0.9 },
        { 8.16, 0.85 },
        { 8.05, 0.8 },
        { 7.97, 0.75 },
        { 7.91, 0.7 },
        { 7.83, 0.65 },
        { 7.75, 0.6 },
        { 7.71, 0.55 },
        { 7.67, 0.5 },
        { 7.63, 0.45 },
        { 7.59, 0.4 },
        { 7.57, 0.35 },
        { 7.53, 0.3 },
        { 7.49, 0.25 },
        { 7.45, 0.2 },
        { 7.41, 0.15 },
        { 7.37, 0.1 },
        { 7.22, 0.05 },
        { 6.55, 0.0 },
      // clang-format on
    });

    struct BatteryState {
        static constexpr std::uint8_t MAX_LEVEL = 255;
        uint8_t level;
    };

    class BatteryLevelEvent : public IEvent {
      public:
        const BatteryState& state;
        BatteryLevelEvent(const BatteryState& state) : IEvent(OH_EVENT_BATTERY_LEVEL), state(state){};
    };
} // namespace SenseShift::Battery
