#pragma once

#include <functional>
#include <utility>
#include <vector>

#include <senseshift/core/component.hpp>
#include <senseshift/input/sensor.hpp>

namespace SenseShift::Body::Hands::Input {
class TotalCurl : public ::SenseShift::Input::FloatSensor {
  public:
    /// \param joints The joints to calculate the total curl from.
    /// \param attach_callbacks Whether to attach callbacks to the joints to update the total curl when they update.
    ///                         If false, the total curl will only be recalculated when the tick() method is called.
    ///                         Setting this to <b>true is not recommended</b>, as it will cause the total curl to
    ///                         be recalculated multiple times per tick (the same as number of joints).
    explicit TotalCurl(std::vector<::SenseShift::Input::FloatSensor> joints, bool attach_callbacks = false) :
      joints_(std::move(joints)), attach_callbacks_(attach_callbacks)
    {
    }

    void init() override
    {
        for (auto& joint : this->joints_) {
            SS_SUBSENSOR_INIT(&joint, this->attach_callbacks_, [this](float /*value*/) {
                this->recalculateState();
            });
        }
    }

    void tick()
    {
        if (this->attach_callbacks_) {
            LOG_E("total_curl", "tick() called when attach_callbacks_ is true, infinite loop go wroom-wroom!");
        }
        this->recalculateState();
    }

    void recalculateState()
    {
        float total = 0.0F;

        for (auto& joint : this->joints_) {
            total += joint.getValue();
        }

        if (!this->joints_.empty()) {
            this->publishState(total / static_cast<float>(this->joints_.size()));
        }
    }

  private:
    std::vector<::SenseShift::Input::FloatSensor> joints_;

    bool attach_callbacks_ = false;
};
} // namespace SenseShift::Body::Hands::Input
