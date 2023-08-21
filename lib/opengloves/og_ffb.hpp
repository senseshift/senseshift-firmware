#pragma once

#include <senseshift/output/actuator.hpp>

#include <optional>

namespace OpenGloves {
    struct HandActuators {
        std::optional<::SenseShift::Output::IActuator<uint16_t>*> thumb = std::nullopt;
        std::optional<::SenseShift::Output::IActuator<uint16_t>*> index = std::nullopt;
        std::optional<::SenseShift::Output::IActuator<uint16_t>*> middle = std::nullopt;
        std::optional<::SenseShift::Output::IActuator<uint16_t>*> ring = std::nullopt;
        std::optional<::SenseShift::Output::IActuator<uint16_t>*> pinky = std::nullopt;
    };
} // namespace OpenGloves
