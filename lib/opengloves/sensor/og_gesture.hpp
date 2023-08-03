#pragma once

#include <sensor/og_finger.hpp>

namespace OpenGloves {
    class Gesture : public OH::ISensor<bool> {};

    class GrabGesture : public Gesture {
      private:
        FingerSensor& index;
        FingerSensor& middle;
        FingerSensor& ring;
        FingerSensor& pinky;
        uint16_t threshold;

      public:
        GrabGesture(FingerSensor& index, FingerSensor& middle, FingerSensor& ring, FingerSensor& pinky, uint16_t threshold) :
          index(index), middle(middle), ring(ring), pinky(pinky), threshold(threshold){};

        void setup() override{};

        bool getValue() override
        {
            return this->index.getCurl() > this->threshold && this->middle.getCurl() > this->threshold
                   && this->ring.getCurl() > this->threshold && this->pinky.getCurl() > this->threshold;
        }
    };

    class TriggerGesture : public Gesture {
      private:
        FingerSensor& index;
        uint16_t threshold;

      public:
        TriggerGesture(FingerSensor& index, uint16_t threshold) : index(index), threshold(threshold){};

        void setup() override{};

        bool getValue() override
        {
            return this->index.getCurl() > this->threshold;
        }
    };

    class PinchGesture : public Gesture {
      private:
        FingerSensor& index;
        FingerSensor& thumb;
        uint16_t threshold;

      public:
        PinchGesture(FingerSensor& index, FingerSensor& thumb, uint16_t threshold) :
          index(index), thumb(thumb), threshold(threshold){};

        void setup() override{};

        bool getValue() override
        {
            return this->index.getCurl() > this->threshold && this->thumb.getCurl() > this->threshold;
        }
    };
} // namespace OpenGloves
