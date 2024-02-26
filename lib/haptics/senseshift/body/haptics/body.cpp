#include "senseshift/body/haptics/body.hpp"
#include "senseshift/body/haptics/interface.hpp"

#include <senseshift/core/logging.hpp>
#include <senseshift/output/output.hpp>

namespace SenseShift::Body::Haptics {
    static const char* const TAG = "haptic.body";

    template<typename Tp, typename Ta>
    void OutputBody<Tp, Ta>::effect(const Target& target, const Position& pos, const typename Plane::Value& val)
    {
        auto plane = this->getTarget(target);
        if (!plane.has_value()) {
            LOG_W(TAG, "No target found for effect: %d", target);
            return;
        }

        plane.value()->effect(pos, val);
    }

    template class OutputBody<Position::Value, Output::IFloatOutput::ValueType>;
} // namespace SenseShift::Body::Haptics
