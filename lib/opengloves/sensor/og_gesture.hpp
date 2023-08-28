#pragma once

#include <sensor/og_finger.hpp>

namespace OpenGloves {
    class Gesture : public SenseShift::Input::ISimpleSensor<bool> {};

    class GrabGesture : public Gesture {
      private:
        ICurl& index;
        ICurl& middle;
        ICurl& ring;
        ICurl& pinky;
        uint16_t threshold;

      public:
        GrabGesture(ICurl& index, ICurl& middle, ICurl& ring, ICurl& pinky, uint16_t threshold) :
          index(index), middle(middle), ring(ring), pinky(pinky), threshold(threshold){};

        void init() override{};

        bool getValue() override
        {
            return this->index.getCurl() > this->threshold && this->middle.getCurl() > this->threshold
                   && this->ring.getCurl() > this->threshold && this->pinky.getCurl() > this->threshold;
        }
    };

    class TriggerGesture : public Gesture {
      private:
        ICurl& index;
        uint16_t threshold;

      public:
        TriggerGesture(ICurl& index, uint16_t threshold) : index(index), threshold(threshold){};

        void init() override{};

        bool getValue() override { return this->index.getCurl() > this->threshold; }
    };

    class PinchGesture : public Gesture {
      private:
        ICurl& index;
        ICurl& thumb;
        uint16_t threshold;

      public:
        PinchGesture(ICurl& index, ICurl& thumb, uint16_t threshold) :
          index(index), thumb(thumb), threshold(threshold){};

        void init() override{};

        bool getValue() override
        {
            return this->index.getCurl() > this->threshold && this->thumb.getCurl() > this->threshold;
        }
    };
} // namespace OpenGloves
