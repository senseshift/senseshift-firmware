#pragma once

#include <cstdint>
#include <variant>

#include <senseshift/math/point2.hpp>

namespace SenseShift::Body::Haptics {
    using EffectIndex = std::uint8_t;
    static constexpr EffectIndex EFFECT_INVALID = 0xFF;
    enum class Effect : EffectIndex {
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
    using Position = Math::Point2<Coordinate>;

    // Vibration intensity_.
    class VibroEffectData {
      public:
        using Intensity = float;

        static constexpr Intensity INTENSITY_MIN = 0.0F;
        static constexpr Intensity INTENSITY_MAX = 1.0F;

        constexpr VibroEffectData() = default;
        constexpr explicit VibroEffectData(const Intensity intensity) : intensity_(intensity) {}
        constexpr VibroEffectData(const VibroEffectData& other) = default;

        constexpr inline explicit operator float() const { return this->intensity_; }

        [[nodiscard]] constexpr inline auto getIntensity() const -> Intensity { return intensity_; };

      private:
        Intensity intensity_ = 0;
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
