#pragma once

#include <algorithm>
#include <iterator>
#include <logging.hpp>

#ifndef constrain
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#endif

namespace OH {
    /**
     * Checks if a container contains a value.
     *
     * @tparam _Cp The type of the container.
     * @tparam _Tp The type of the value.
     */
    template<class _Cp, typename _Tp>
    inline bool contains(_Cp&& c, _Tp val)
    {
        return std::find(std::begin(c), std::end(c), val) != std::end(c);
    };

    template<typename _Tp>
    inline bool contains(_Tp* begin, _Tp* end, const _Tp& val)
    {
        return std::find(begin, end, val) != end;
    };

    template<typename _Tp>
    constexpr inline bool contains(const _Tp* arr, const std::size_t size, const _Tp& val)
    {
        return std::find(arr, arr + size, val) != arr + size;
    };

    template<typename _Tp>
    constexpr _Tp accurateMap(_Tp x, _Tp in_min, _Tp in_max, _Tp out_min, _Tp out_max)
    {
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
        return x * out_max / in_max;
    }
} // namespace OH
