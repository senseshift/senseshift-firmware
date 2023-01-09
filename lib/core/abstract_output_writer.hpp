#pragma once

#include <stdint.h>

typedef uint16_t oh_outputIntensity_t;

namespace OH
{

  //! Singular output point (e.g. vibration motor)
  class AbstractOutputWriter {
   public:
    virtual void setup(){};
    virtual void writeOutput(oh_outputIntensity_t intensity) = 0;
  };
} // namespace OH
