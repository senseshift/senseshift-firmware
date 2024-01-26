#include "senseshift/body/haptics/body.hpp"
#include "senseshift/body/haptics/interface.hpp"

#include <senseshift/output/output.hpp>
#include <senseshift/core/logging.hpp>

namespace SenseShift::Body::Haptics {
    static const char *const TAG = "haptic.body";

    template<typename Tp, typename Ta>
    void OutputBody<Tp, Ta>::effect(const Target& target, const Position& pos, const typename Plane::Value& val)
    {
        auto find = this->targets_.find(target);
        if (find == this->targets_.end()) {
            LOG_W(TAG, "No target found for effect: %d", target);
            return;
        }

        find->second->effect(pos, val);
    }

    template class OutputBody<Position::Value, Output::FloatOutput::ValueType>;
} // namespace SenseShift::Body::Haptics
