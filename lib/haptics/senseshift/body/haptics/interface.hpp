#pragma once

#include <cstdint>
#include <variant>

#include <senseshift/math/point2.hpp>

namespace SenseShift::Body::Haptics {
    using EffectIntex = std::uint8_t;
    static constexpr EffectIntex EFFECT_INVALID = 0xFF;
    enum class Effect : EffectIntex {
        Invalid = EFFECT_INVALID,
        Vibro = 0x00,
        // TODO: thermal, etc.
        // Thermal = 0x01,
    };

    using TargetIndex = std::uint8_t;
    static constexpr TargetIndex TARGET_INVALID = 0xFF;
    enum class Target : TargetIndex {
        Invalid = TARGET_INVALID,
        ChestFront = 0x00,
        ChestBack = 0x01,

        // Legacy backword compatibility
        Accessory [[deprecated]] = 0x02,

        FaceFront,

        HandLeftThumb,
        HandLeftIndex,
        HandLeftMiddle,
        HandLeftRing,
        HandLeftLittle,
        HandLeftVolar,  // Palm
        HandLeftDorsal, // Back

        HandRightThumb,
        HandRightIndex,
        HandRightMiddle,
        HandRightRing,
        HandRightLittle,
        HandRightVolar,  // Palm
        HandRightDorsal, // Back

        // TODO: arms, legs, etc.
    };

    using Coordinate = std::uint8_t;
    using Position = ::SenseShift::Math::Point2<Coordinate>;

    // Vibration intensity.
    struct VibroEffectData {
        using Intensity = std::uint16_t;
        static constexpr Intensity INTENSITY_MIN = 0;
        static constexpr Intensity INTENSITY_MAX = 4095;

        Intensity intensity = 0;

        inline constexpr VibroEffectData() = default;
        inline constexpr VibroEffectData(const Intensity intensity) : intensity(intensity) {}
        inline constexpr VibroEffectData(const VibroEffectData& other) = default;

        inline constexpr operator std::uint16_t() const { return intensity; }
    };

    // TODO: thermal, etc.

    using EffectData = std::variant<VibroEffectData>;

    struct EffectRequest {
        Effect effect = Effect::Invalid;
        Target target = Target::Invalid;
        Position position = Position(0, 0);
        EffectData data;
    };
} // namespace SenseShift::Body::Haptics
