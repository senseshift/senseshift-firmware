#pragma once

#include <optional>

#include "og_sensor.hpp"

namespace OpenGloves {
    struct FingerSensors {
        std::vector<SenseShift::Input::CalibratedSimpleSensor<uint16_t>*> curl =
          std::vector<SenseShift::Input::CalibratedSimpleSensor<uint16_t>*>();
        std::optional<SenseShift::Input::CalibratedSimpleSensor<uint16_t>*> splay = std::nullopt;

        FingerSensors(
          std::vector<SenseShift::Input::CalibratedSimpleSensor<uint16_t>*> curl,
          std::optional<SenseShift::Input::CalibratedSimpleSensor<uint16_t>*> splay = std::nullopt
        ) :
          curl(curl), splay(splay){};

        FingerSensors(
          SenseShift::Input::CalibratedSimpleSensor<uint16_t>* curl1,
          std::optional<SenseShift::Input::CalibratedSimpleSensor<uint16_t>*> splay = std::nullopt
        ) :
          curl({ curl1 }), splay(splay){};
    };

    class ICurl {
      public:
        /**
         * Get finger curl value.
         */
        virtual uint16_t getCurl() = 0;
    };

    typedef SenseShift::Input::ISimpleSensor<FingerValue> IFingerSensor;

    class SimpleFingerSensor : public IFingerSensor, public ICurl {
      public:
        SimpleFingerSensor(FingerSensors sensors) : sensors(sensors){};

        SimpleFingerSensor(
          SenseShift::Input::CalibratedSimpleSensor<uint16_t>* curl1,
          std::optional<SenseShift::Input::CalibratedSimpleSensor<uint16_t>*> splay = std::nullopt
        ) :
          sensors(curl1, splay){};

        void init() override
        {
            for (auto sensor : sensors.curl) {
                sensor->init();
            }
            if (sensors.splay.has_value()) {
                sensors.splay.value()->init();
            }
        }

        FingerValue getValue() override
        {
            FingerValue value{
                .curl = std::vector<uint16_t>(),
                .splay = std::nullopt,
            };
            for (auto sensor : sensors.curl) {
                value.curl.push_back(sensor->getValue());
            }
            if (sensors.splay.has_value()) {
                value.splay = sensors.splay.value()->getValue();
            }
            return value;
        }

        uint16_t getCurl() override { return this->getValue().getTotalCurl(); }

      protected:
        FingerSensors sensors;
    };

    /**
     * Simple finger sensor that only provides calibration.
     */
    class CalibratedFingerSensor : public SimpleFingerSensor, public SenseShift::Calibration::ICalibrated {
      public:
        CalibratedFingerSensor(FingerSensors sensors) : SimpleFingerSensor(sensors){};

        CalibratedFingerSensor(
          SenseShift::Input::CalibratedSimpleSensor<uint16_t>* curl1,
          std::optional<SenseShift::Input::CalibratedSimpleSensor<uint16_t>*> splay = std::nullopt
        ) :
          SimpleFingerSensor(curl1, splay){};

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

    class FingerSensor :
      public StringEncodedMemoizedSensor<FingerValue>,
      public SenseShift::Calibration::ICalibrated,
      public ICurl {
      public:
        FingerSensor(CalibratedFingerSensor* sensor, IEncodedInput::Type type) :
          StringEncodedMemoizedSensor<FingerValue>(sensor, type){};

        FingerSensor(
          SenseShift::Input::CalibratedSimpleSensor<uint16_t>* curl1,
          std::optional<SenseShift::Input::CalibratedSimpleSensor<uint16_t>*> splay,
          IEncodedInput::Type type
        ) :
          StringEncodedMemoizedSensor<FingerValue>(new CalibratedFingerSensor(curl1, splay), type){};

        FingerSensor(SenseShift::Input::CalibratedSimpleSensor<uint16_t>* curl1, IEncodedInput::Type type) :
          StringEncodedMemoizedSensor<FingerValue>(new CalibratedFingerSensor(curl1, std::nullopt), type){};

        void resetCalibration() override { static_cast<CalibratedFingerSensor*>(this->sensor)->resetCalibration(); }

        void enableCalibration() override { static_cast<CalibratedFingerSensor*>(this->sensor)->enableCalibration(); }

        void disableCalibration() override { static_cast<CalibratedFingerSensor*>(this->sensor)->disableCalibration(); }

        uint16_t getCurl() override { return this->getValue().getTotalCurl(); }
    };

    struct HandSensors {
        std::optional<FingerSensor> thumb = std::nullopt;
        std::optional<FingerSensor> index = std::nullopt;
        std::optional<FingerSensor> middle = std::nullopt;
        std::optional<FingerSensor> ring = std::nullopt;
        std::optional<FingerSensor> pinky = std::nullopt;
    };

} // namespace OpenGloves
