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
  bool contains(_Cp&& c, _Tp t) {
    return std::find(std::begin(c), std::end(c), t) != std::end(c);
  };
}  // namespace OH
