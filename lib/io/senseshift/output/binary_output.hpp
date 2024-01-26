#pragma once

#include "senseshift/output/output.hpp"

namespace SenseShift::Output {
    class BinaryOutput : public ::SenseShift::Output::Output<bool> {
      public:
        virtual void writeOutput(bool) = 0;
    };
} // namespace SenseShift::Output