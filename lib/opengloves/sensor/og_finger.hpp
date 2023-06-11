#pragma once

#include "og_sensor.hpp"

namespace OpenGloves
{
  class FingerSensor : public StringEncodedMemoizedSensor<uint16_t>, public virtual OH::ICalibrated {
    public :
      FingerSensor(OH::CalibratedSensor<uint16_t>* sensor, IEncodedInput::Type type) : StringEncodedMemoizedSensor<uint16_t>(sensor, type) { };

      void resetCalibration() override {
        static_cast<OH::CalibratedSensor<uint16_t>*>(this->sensor)->resetCalibration();
      }

      void enableCalibration() override {
        static_cast<OH::CalibratedSensor<uint16_t>*>(this->sensor)->enableCalibration();
      }

      void disableCalibration() override {
        static_cast<OH::CalibratedSensor<uint16_t>*>(this->sensor)->disableCalibration();
      }
  };

  // TODO: add splay finger sensor

} // namespace OpenGloves
