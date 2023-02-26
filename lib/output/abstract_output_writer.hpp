#pragma once

#include <stdint.h>

#define OH_OUTPUT_INTENSITY_T uint8_t
#define OH_OUTPUT_INTENSITY_MAX UINT8_MAX

typedef OH_OUTPUT_INTENSITY_T oh_output_intensity_t;

namespace OH {
  //! Singular output point (e.g. vibration motor)
  class AbstractOutputWriter {
   public:
    virtual void setup(){};
    virtual void writeOutput(oh_output_intensity_t intensity) = 0;
  };
}  // namespace OH
