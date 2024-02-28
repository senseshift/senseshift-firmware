#pragma once

#include <senseshift/core/component.hpp>
#include <senseshift/input/sensor/analog_threshold.hpp>
#include <senseshift/input/sensor/sensor.hpp>

namespace SenseShift::Body::Hands::Input {
    using Gesture = ::SenseShift::Input::BinarySensor;

    /// An alias for semantic consistency.
    using TriggerGesture = ::SenseShift::Input::AnalogThresholdSensor<float>;

    class GrabGesture : public Gesture {
      public:
        struct Fingers {
            ::SenseShift::Input::FloatSensor* index;
            ::SenseShift::Input::FloatSensor* middle;
            ::SenseShift::Input::FloatSensor* ring;
            ::SenseShift::Input::FloatSensor* pinky;
        };

        explicit GrabGesture(Fingers fingers, float threshold = 0.5F, bool attach_callbacks = false) :
          fingers_(fingers), threshold_(threshold), attach_callbacks_(attach_callbacks)
        {
        }

        void init() override
        {
            SS_SUBSENSOR_INIT(this->fingers_.index, this->attach_callbacks_, [this](float /*value*/) {
                this->recalculateState();
            });
            SS_SUBSENSOR_INIT(this->fingers_.middle, this->attach_callbacks_, [this](float /*value*/) {
                this->recalculateState();
            });
            SS_SUBSENSOR_INIT(this->fingers_.ring, this->attach_callbacks_, [this](float /*value*/) {
                this->recalculateState();
            });
            SS_SUBSENSOR_INIT(this->fingers_.pinky, this->attach_callbacks_, [this](float /*value*/) {
                this->recalculateState();
            });
        }

        void tick() override
        {
            if (this->attach_callbacks_) {
                LOG_E("gesture.grab", "tick() called when attach_callbacks_ is true, infinite loop go wroom-wroom!");
            }
            this->recalculateState();
        }

        void recalculateState()
        {
            return this->publishState(
              this->fingers_.index->getValue() > this->threshold_
              && this->fingers_.middle->getValue() > this->threshold_
              && this->fingers_.ring->getValue() > this->threshold_
              && this->fingers_.pinky->getValue() > this->threshold_
            );
        }

      private:
        Fingers fingers_;
        float threshold_;
        bool attach_callbacks_ = false;
    };

    class PinchGesture : public Gesture {
      public:
        struct Fingers {
            ::SenseShift::Input::FloatSensor* thumb;
            ::SenseShift::Input::FloatSensor* index;
        };

        explicit PinchGesture(Fingers fingers, float threshold = 0.5F, bool attach_callbacks = false) :
          fingers_(fingers), threshold_(threshold), attach_callbacks_(attach_callbacks)
        {
        }

        void init() override
        {
            SS_SUBSENSOR_INIT(this->fingers_.thumb, this->attach_callbacks_, [this](float /*value*/) {
                this->recalculateState();
            });
            SS_SUBSENSOR_INIT(this->fingers_.index, this->attach_callbacks_, [this](float /*value*/) {
                this->recalculateState();
            });
        }

        void tick() override
        {
            if (this->attach_callbacks_) {
                LOG_E("gesture.pinch", "tick() called when attach_callbacks_ is true, infinite loop go wroom-wroom!");
            }
            this->recalculateState();
        }

        void recalculateState()
        {
            return this->publishState(
              this->fingers_.thumb->getValue() > this->threshold_ && this->fingers_.index->getValue() > this->threshold_
            );
        }

      private:
        Fingers fingers_;
        float threshold_;
        bool attach_callbacks_ = false;
    };
} // namespace SenseShift::Body::Hands::Input