#pragma once

#include <stdint.h>
#include <variant>

#include <senseshift/math/point2.hpp>

namespace SenseShift::Body::Haptics {
    typedef uint8_t EffectIntex_t;
    static const EffectIntex_t EFFECT_INVALID = 0xFF;
    typedef enum class Effect : EffectIntex_t {
        Invalid = EFFECT_INVALID,
        Vibro = 0x00,
        // TODO: thermal, etc.
        // Thermal = 0x01,
    } Effect_t;

    typedef uint8_t TargetIndex_t;
    static const TargetIndex_t TARGET_INVALID = 0xFF;
    typedef enum class Target : TargetIndex_t {
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
    } Target_t;

    typedef uint8_t Coordinate_t;
    typedef ::SenseShift::Math::Point2<Coordinate_t> Position_t;

    // Vibration intensity.
    typedef struct VibroEffectData {
        using Intensity_t = uint16_t;
        inline static const Intensity_t INTENSITY_MIN = 0;
        inline static const Intensity_t INTENSITY_MAX = 4095;

        Intensity_t intensity = 0;

        inline constexpr VibroEffectData() = default;
        inline constexpr VibroEffectData(const Intensity_t intensity) : intensity(intensity) {}
        inline constexpr VibroEffectData(const VibroEffectData& other) = default;

        inline constexpr operator uint16_t() const { return intensity; }
    } VibroEffectData_t;

    // TODO: thermal, etc.

    typedef std::variant<VibroEffectData_t
                         // TODO: thermal, etc.
                         // ThermalEffectData_t
                         >
      EffectData_t;

    typedef struct EffectRequest {
        Effect_t effect = Effect::Invalid;
        Target_t target = Target::Invalid;
        Position_t position = Position_t(0, 0);
        EffectData_t data;
    } EffectRequest_t;
} // namespace SenseShift::Body::Haptics
