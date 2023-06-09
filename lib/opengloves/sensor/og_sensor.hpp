#pragma once

#include <sensor.hpp>
#include <calibration.hpp>
#include <opengloves_protocol.hpp>

namespace OpenGloves
{
  class IEncodedSensor : public EncodedInput {
    public:
      IEncodedSensor(Type type) : EncodedInput(type) { };
      virtual void setup() = 0;
      virtual void updateValue() = 0;
  };

  struct ICalibratedEncodedSensor : public IEncodedSensor {
    public:
      ICalibratedEncodedSensor(Type type) : IEncodedSensor(type) { };
      virtual void resetCalibration() = 0;
      virtual void enableCalibration() = 0;
      virtual void disableCalibration() = 0;
  };
} // namespace OpenGloves
