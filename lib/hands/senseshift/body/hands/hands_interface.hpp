#pragma once

#include <senseshift/body/haptics/interface.hpp>

#include <cstdint>

namespace SenseShift::Body::Hands {
    using HandLateralityIndex = std::uint8_t;
    enum class HandSide : HandLateralityIndex { Left, Right };
    using FingerIndex = std::uint8_t;
    enum class Finger : FingerIndex {
        Thumb,
        Index,
        Middle,
        Ring,
        Little,
    };

    namespace Haptics {
        /// @brief Helper with position of the haptic device on the fingertip.
        /// Distal phalanx of the volar surface of the any finger.
        static constexpr const ::SenseShift::Body::Haptics::Position FINGERTIP_POSITION(127, 16);
    } // namespace Haptics
} // namespace SenseShift::Body::Hands
