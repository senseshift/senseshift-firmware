#pragma once

#include <algorithm>
#include <iterator>
#include <type_traits>

namespace SenseShift {
    /**
     * Checks if a container contains a value.
     *
     * @tparam _Cp The type of the container.
     * @tparam _Tp The type of the value.
     */
    template<class _Cp, typename _Tp>
    inline auto contains(const _Cp&& c, const _Tp val) -> bool
    {
        return std::find(std::begin(c), std::end(c), val) != std::end(c);
    };

    template<>
    inline bool contains<std::string&, char>(std::string& s, char val)
    {
        return s.find(val) != std::string::npos;
    };

    template<typename _Tp>
    inline bool contains(_Tp* begin, _Tp* end, const _Tp& val)
    {
        return std::find(begin, end, val) != end;
    };

    template<typename _Tp>
    constexpr inline bool contains(const _Tp* arr, const std::size_t size, const _Tp& val)
    {
        static_assert(
          std::is_same<_Tp, typename std::iterator_traits<_Tp*>::value_type>::value,
          "Container and value must be of the same type"
        );
        return std::find(arr, arr + size, val) != arr + size;
    };
} // namespace SenseShift
