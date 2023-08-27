#pragma once

#include <cstdint>
#include <senseshift/body/haptics/interface.hpp>

namespace SenseShift::Body {
    namespace Hands {
        typedef std::uint8_t HandSideIndex_t;
        typedef enum class HandSide : HandSideIndex_t { Left, Right } HandSide_t;
        typedef std::uint8_t FingerIndex_t;
        typedef enum class Finger : FingerIndex_t {
            Thumb,
            Index,
            Middle,
            Ring,
            Little,
        } Finger_t;

        namespace Haptics {
            /**
             * @brief Helper with position of the haptic device on the fingertip.
             * Distal phalanx of the volar surface of the any finger.
             */
            static constexpr const ::SenseShift::Body::Haptics::Position_t FINGERTIP_POSITION(127, 16);
        } // namespace Haptics
    }     // namespace Hands
} // namespace SenseShift::Body
