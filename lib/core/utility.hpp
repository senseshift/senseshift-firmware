#pragma once

#include <algorithm>
#include <iterator>

namespace OH {
  /**
   * Checks if a container contains a value.
   *
   * @tparam _Cp The type of the container.
   * @tparam _Tp The type of the value.
   */
  template <class _Cp, typename _Tp>
  inline bool contains(_Cp&& c, _Tp val) {
    return std::find(std::begin(c), std::end(c), val) != std::end(c);
  };

  template <typename _Tp>
  inline bool contains(_Tp* begin, _Tp* end, const _Tp& val) {
    return std::find(begin, end, val) != end;
  };

  template <typename _Tp>
  inline bool contains(const _Tp* arr, const std::size_t size, const _Tp& val) {
    return std::find(arr, arr + size, val) != arr + size;
  };
}  // namespace OH
