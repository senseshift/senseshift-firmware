#pragma once

#include <sensor.hpp>
#include <calibration.hpp>
#include <opengloves_protocol.hpp>

#include <Arduino.h>

namespace OpenGloves
{
  struct IFinger {
    //
  };

  class FingerSensor : public IFinger, public OH::CalibratedSensor<uint16_t>, public EncodedInput
  {
   private:
    EncodedInput::Type type;

   public:
    FingerSensor(ISensor<uint16_t>* sensor, OH::Calibrator<uint16_t>* calibrator, EncodedInput::Type type)
      : CalibratedSensor<uint16_t>(sensor, calibrator), type(type) {}
  };

  // TODO: add splay finger sensor

} // namespace OpenGloves
