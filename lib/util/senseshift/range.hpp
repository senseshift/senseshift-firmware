#include <cstdint>
#include <type_traits>

#include <senseshift/logging.hpp>

namespace SenseShift {
    template<typename _Tp>
    constexpr _Tp accurateMap(_Tp x, _Tp in_min, _Tp in_max, _Tp out_min, _Tp out_max)
    {
        static_assert(std::is_arithmetic<_Tp>::value, "Type must be arithmetic");

        const _Tp run = in_max - in_min;
        if (run == 0) {
            log_e("map(): Invalid input range, min == max");
            return (out_min + out_max) / 2;
        }
        const _Tp rise = out_max - out_min;
        const _Tp delta = x - in_min;
        return (delta * rise) / run + out_min;
    }

    // Same as the above, but both mins are 0.
    template<typename _Tp>
    constexpr inline _Tp simpleMap(_Tp x, _Tp in_max, _Tp out_max)
    {
        static_assert(std::is_arithmetic<_Tp>::value, "Type must be arithmetic");

        return x * out_max / in_max;
    }
} // namespace SenseShift
