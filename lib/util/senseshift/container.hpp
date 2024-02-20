#pragma once

#include <algorithm>
#include <iterator>
#include <string>
#include <type_traits>

namespace SenseShift {
    /**
     * Checks if a container contains a value.
     *
     * @tparam _Cp The type of the container.
     * @tparam _Tp The type of the value.
     */
    template<class Cp, typename Tp>
    inline auto contains(Cp&& c, Tp val) -> bool
    {
        return std::find(std::begin(c), std::end(c), val) != std::end(c);
    };

    template<>
    inline auto contains<std::string&, char>(std::string& s, char val) -> bool
    {
        return s.find(val) != std::string::npos;
    };

    template<typename Tp>
    inline auto contains(Tp* begin, Tp* end, const Tp& val) -> bool
    {
        return std::find(begin, end, val) != end;
    };

    template<typename Tp>
    constexpr inline auto contains(const Tp* arr, const std::size_t size, const Tp& val) -> bool
    {
        static_assert(
          std::is_same_v<Tp, typename std::iterator_traits<Tp*>::value_type>,
          "Container and value must be of the same type"
        );
        return std::find(arr, arr + size, val) != arr + size;
    };
} // namespace SenseShift
