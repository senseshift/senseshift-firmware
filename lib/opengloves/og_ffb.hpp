#include <abstract_actuator.hpp>

#include <optional>

namespace OpenGloves
{
    struct HandActuators
    {
        std::optional<OH::AbstractActuator*> thumb = std::nullopt;
        std::optional<OH::AbstractActuator*> index = std::nullopt;
        std::optional<OH::AbstractActuator*> middle = std::nullopt;
        std::optional<OH::AbstractActuator*> ring = std::nullopt;
        std::optional<OH::AbstractActuator*> pinky = std::nullopt;
    };
} // namespace OpenGloves
