#pragma once

#include <senseshift/core/component.hpp>

namespace SenseShift::Output {
template<typename Tp>
class IOutput : public IInitializable {
  public:
    using ValueType = Tp;

    virtual void writeState(ValueType value) = 0;
};

using IBinaryOutput = IOutput<bool>;
using IFloatOutput = IOutput<float>;
} // namespace SenseShift::Output
