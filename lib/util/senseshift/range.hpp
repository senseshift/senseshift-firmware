#include <cstdint>
#include <type_traits>

#include <senseshift/core/logging.hpp>
#include <senseshift/core/helpers.hpp>

namespace SenseShift {
    /// Remap \p value from the range (\p min, \p max) to (\p min_out, \p max_out).
    template<typename Tp>
    constexpr auto accurateMap(Tp value, Tp min, Tp max, Tp min_out, Tp max_out) noexcept -> Tp
    {
        return remap<Tp, Tp>(value, min, max, min_out, max_out);
    }

    // Same as the above, but both mins are 0.
    template<typename Tp>
    constexpr auto simpleMap(Tp value, Tp in_max, Tp out_max) noexcept -> Tp
    {
        return remap_simple<Tp, Tp>(value, in_max, out_max);
    }
} // namespace SenseShift
