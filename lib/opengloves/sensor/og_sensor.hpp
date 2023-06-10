#pragma once

#include <sensor.hpp>
#include <calibration.hpp>
#include <og_protocol.hpp>

namespace OpenGloves
{
  struct ICalibratedEncodedSensor : public IEncodedSensor {
    public:
      ICalibratedEncodedSensor(Type type) : IEncodedSensor(type) { };
      virtual void resetCalibration() = 0;
      virtual void enableCalibration() = 0;
      virtual void disableCalibration() = 0;
  };
} // namespace OpenGloves
