#include "senseshift/body/haptics/body.hpp"

#include <senseshift/logging.hpp>

namespace SenseShift::Body::Haptics {
    void HapticBody::setup()
    {
        for (auto& [target, plane] : this->vibroTargets) {
            plane->setup();
        }
    }

    void HapticBody::effect(const EffectRequest_t& effect)
    {
        if (effect.effect == Effect::Vibro && std::holds_alternative<VibroEffectData_t>(effect.data)) {
            auto it = this->vibroTargets.find(effect.target);
            if (it == this->vibroTargets.end()) {
                log_w("No target found for effect: %d", effect.target);
                return;
            }

            it->second->effect(effect.position, std::get<VibroEffectData_t>(effect.data));
        } else {
            log_w("Non-supported effect type: %d", effect.effect);
        }
    }

    void HapticBody::addTarget(const Target_t target, VibroPlane* plane)
    {
        this->vibroTargets[target] = plane;
        this->allTargets.insert({ target, plane });
    }
} // namespace SenseShift::Body::Haptics
