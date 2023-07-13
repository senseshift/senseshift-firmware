#pragma once

#include <optional>

#include "og_sensor.hpp"

namespace OpenGloves {
    class ICurl {
      public:
        virtual uint16_t getCurl() = 0;
    };

    class IFinger : public StringEncodedMemoizedSensor<uint16_t>, public virtual OH::ICalibrated, public virtual ICurl {
      public:
        IFinger(OH::CalibratedSensor<uint16_t>* sensor, IEncodedInput::Type type) :
          StringEncodedMemoizedSensor<uint16_t>(sensor, type){};

        uint16_t getCurl() override
        {
            return this->getValue();
        }
    };

    class FingerSensor : public IFinger {
      public:
        FingerSensor(OH::CalibratedSensor<uint16_t>* sensor, IEncodedInput::Type type) : IFinger(sensor, type){};

        void resetCalibration() override
        {
            static_cast<OH::CalibratedSensor<uint16_t>*>(this->sensor)->resetCalibration();
        }

        void enableCalibration() override
        {
            static_cast<OH::CalibratedSensor<uint16_t>*>(this->sensor)->enableCalibration();
        }

        void disableCalibration() override
        {
            static_cast<OH::CalibratedSensor<uint16_t>*>(this->sensor)->disableCalibration();
        }
    };

    // TODO: add splay finger sensor

    struct HandSensors {
        std::optional<FingerSensor> thumb = std::nullopt;
        std::optional<FingerSensor> index = std::nullopt;
        std::optional<FingerSensor> middle = std::nullopt;
        std::optional<FingerSensor> ring = std::nullopt;
        std::optional<FingerSensor> pinky = std::nullopt;
    };

} // namespace OpenGloves
