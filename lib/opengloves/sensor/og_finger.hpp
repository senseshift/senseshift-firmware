#pragma once

#include <optional>

#include "og_sensor.hpp"

namespace OpenGloves {
    struct FingerSensors {
        std::vector<OH::CalibratedSensor<uint16_t>*> curl = std::vector<OH::CalibratedSensor<uint16_t>*>();
        std::optional<OH::CalibratedSensor<uint16_t>*> splay = std::nullopt;

        FingerSensors(std::vector<OH::CalibratedSensor<uint16_t>*> curl, std::optional<OH::CalibratedSensor<uint16_t>*> splay = std::nullopt) :
            curl(curl), splay(splay){};

        FingerSensors(OH::CalibratedSensor<uint16_t>* curl1, std::optional<OH::CalibratedSensor<uint16_t>*> splay = std::nullopt) :
            curl({ curl1 }), splay(splay){};
    };

    class ICurl {
      public:
        /**
         * Get finger curl value.
         */
        virtual uint16_t getCurl() = 0;
    };

    typedef OH::ISensor<FingerValue> IFingerSensor;

    class SimpleFingerSensor : public IFingerSensor, public ICurl {
      public:
        SimpleFingerSensor(FingerSensors sensors) : sensors(sensors){};

        SimpleFingerSensor(OH::CalibratedSensor<uint16_t>* curl1, std::optional<OH::CalibratedSensor<uint16_t>*> splay = std::nullopt) : sensors(curl1, splay){};

        void setup() override
        {
            for (auto sensor : sensors.curl) {
                sensor->setup();
            }
            if (sensors.splay.has_value()) {
                sensors.splay.value()->setup();
            }
        }

        FingerValue getValue() override
        {
            FingerValue value;
            for (auto sensor : sensors.curl) {
                value.curl.push_back(sensor->getValue());
            }
            if (sensors.splay.has_value()) {
                value.splay = sensors.splay.value()->getValue();
            }
            return value;
        }

        uint16_t getCurl() override
        {
            uint16_t value = 0;
            for (auto sensor : sensors.curl) {
                value += sensor->getValue();
            }
            return value / sensors.curl.size();
        }

      protected:
        FingerSensors sensors;
    };

    /**
     * Simple finger sensor that only provides calibration.
     */
    class CalibratedFingerSensor : public SimpleFingerSensor, public OH::ICalibrated {
      public:
        CalibratedFingerSensor(FingerSensors sensors) : SimpleFingerSensor(sensors){};

        CalibratedFingerSensor(OH::CalibratedSensor<uint16_t>* curl1, std::optional<OH::CalibratedSensor<uint16_t>*> splay = std::nullopt) : SimpleFingerSensor(curl1, splay){};

        void resetCalibration() override
        {
            for (auto sensor : this->sensors.curl) {
                sensor->resetCalibration();
            }
            if (this->sensors.splay.has_value()) {
                this->sensors.splay.value()->resetCalibration();
            }
        }

        void enableCalibration() override
        {
            for (auto sensor : this->sensors.curl) {
                sensor->enableCalibration();
            }
            if (this->sensors.splay.has_value()) {
                this->sensors.splay.value()->enableCalibration();
            }
        }

        void disableCalibration() override
        {
            for (auto sensor : this->sensors.curl) {
                sensor->disableCalibration();
            }
            if (this->sensors.splay.has_value()) {
                this->sensors.splay.value()->disableCalibration();
            }
        }
    };

    class FingerSensor : public StringEncodedMemoizedSensor<FingerValue>, public OH::ICalibrated, public ICurl {
      public:
        FingerSensor(CalibratedFingerSensor* sensor, IEncodedInput::Type type) :
          StringEncodedMemoizedSensor<FingerValue>(sensor, type){};

        FingerSensor(OH::CalibratedSensor<uint16_t>* curl1, std::optional<OH::CalibratedSensor<uint16_t>*> splay, IEncodedInput::Type type) :
          StringEncodedMemoizedSensor<FingerValue>(new CalibratedFingerSensor(curl1, splay), type){};

        FingerSensor(OH::CalibratedSensor<uint16_t>* curl1, IEncodedInput::Type type) :
          StringEncodedMemoizedSensor<FingerValue>(new CalibratedFingerSensor(curl1, std::nullopt), type){};

        void resetCalibration() override
        {
            static_cast<CalibratedFingerSensor*>(this->sensor)->resetCalibration();
        }

        void enableCalibration() override
        {
            static_cast<CalibratedFingerSensor*>(this->sensor)->enableCalibration();
        }

        void disableCalibration() override
        {
            static_cast<CalibratedFingerSensor*>(this->sensor)->disableCalibration();
        }

        uint16_t getCurl() override
        {
            return static_cast<CalibratedFingerSensor*>(this->sensor)->getCurl();
        }
    };

    struct HandSensors {
        std::optional<FingerSensor> thumb = std::nullopt;
        std::optional<FingerSensor> index = std::nullopt;
        std::optional<FingerSensor> middle = std::nullopt;
        std::optional<FingerSensor> ring = std::nullopt;
        std::optional<FingerSensor> pinky = std::nullopt;
    };

} // namespace OpenGloves
