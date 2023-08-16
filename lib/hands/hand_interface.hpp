#pragma once

#include <cstdint>
#include <haptics_interface.hpp>

namespace SenseShift::Body {
    namespace Hands {
        typedef std::uint8_t HandPositionIndex_t;
        typedef enum class HandPosition : HandPositionIndex_t { Left, Right } HandPosition_t;

        namespace Haptics {
            /**
             * @brief Helper with position of the haptic device on the fingertip.
             * Distal phalanx of the volar surface of the any finger.
             */
            static constexpr const ::SenseShift::Body::Haptics::Position_t FINGERTIP_POSITION(127, 16);
        } // namespace Haptics
    }     // namespace Hands
} // namespace SenseShift::Body
