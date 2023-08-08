#pragma once

#include <senseshift_types.hpp>

namespace OH {
    //! Singular output point (e.g. vibration motor)
    class AbstractActuator {
      public:
        virtual void setup(){};
        virtual void writeOutput(oh_output_intensity_t intensity) = 0;
    };
} // namespace OH
