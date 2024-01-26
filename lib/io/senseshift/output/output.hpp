#pragma once

#include <senseshift/core/component.hpp>

namespace SenseShift::Output {
    template<typename Tp>
    class Output : public IInitializable {
    public:
        using ValueType = Tp;

        virtual void writeState(ValueType value) = 0;
    };

    using FloatOutput = Output<float>;
} // namespace SenseShift::Output