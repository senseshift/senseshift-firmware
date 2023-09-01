#pragma once

#include <cstdint>

#include <senseshift/logging.hpp>

namespace SenseShift::Output {
    // Singular output point (e.g. vibration motor)
    template<typename T>
    class IActuator {
      public:
        virtual void setup(){};
        virtual void writeOutput(T) = 0;
    };

    template<>
    class IActuator<std::uint16_t> {
      public:
        virtual void setup(){};
        virtual void writeOutput(std::uint16_t) = 0;
    };
} // namespace SenseShift::Output
