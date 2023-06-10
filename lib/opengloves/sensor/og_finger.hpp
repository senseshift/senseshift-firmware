#pragma once

#include "og_sensor.hpp"

namespace OpenGloves
{
  class IFinger : public ICalibratedEncodedSensor, public OH::MemoizedSensor<uint16_t> {
    public :
      IFinger(OH::CalibratedSensor<uint16_t>* sensor, IEncodedInput::Type type)
        : ICalibratedEncodedSensor(type), OH::MemoizedSensor<uint16_t>(sensor) { };
  };

  class FingerSensor : public IFinger
  {
   public:
    FingerSensor(OH::CalibratedSensor<uint16_t>* sensor, IEncodedInput::Type type) : IFinger(sensor, type) { };

    void setup() override {
      this->sensor->setup();
    }

    void updateValue() override {
      this->value = this->sensor->getValue();
    }

    void resetCalibration() override {
      static_cast<OH::CalibratedSensor<uint16_t>*>(this->sensor)->resetCalibration();
    }

    void enableCalibration() override {
      static_cast<OH::CalibratedSensor<uint16_t>*>(this->sensor)->enableCalibration();
    }

    void disableCalibration() override {
      static_cast<OH::CalibratedSensor<uint16_t>*>(this->sensor)->disableCalibration();
    }

    size_t getEncodedSize() const override {
      // Encode string size = AXXXX + '\0'
      return 6;
    }

    int encode(char* buffer) const override {
      return snprintf(buffer, this->getEncodedSize(), "%c%04d", this->getType(), this->value);
    }
  };

  // TODO: add splay finger sensor

} // namespace OpenGloves
