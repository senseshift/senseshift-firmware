#pragma once

#include <type_traits>

namespace SenseShift {
    /// @name Mathematics
    /// @{

    /// Linearly interpolate between \p start and \p end by \p completion (between 0 and 1).
    constexpr auto lerp(const float completion, const float start, const float end) -> float
    {
        return start + (end - start) * completion;
    }

    /// Remap \p value from the range (\p min, \p max) to (\p min_out, \p max_out).
    ///
    /// \tparam Tp The output type.
    /// \tparam Up The input type.
    ///
    /// \param value The value to remap.
    /// \param min The minimum value of the input range.
    /// \param max The maximum value of the input range.
    /// \param min_out The minimum value of the output range.
    /// \param max_out The maximum value of the output range.
    ///
    /// \return The remapped value.
    template<typename Tp, typename Up>
    constexpr auto remap(Up value, Up min, Up max, Tp min_out, Tp max_out) -> Tp
    {
        static_assert(std::is_arithmetic_v<Up>, "Type must be arithmetic");
        static_assert(std::is_arithmetic_v<Tp>, "Type must be arithmetic");

        return (value - min) * (max_out - min_out) / (max - min) + min_out;
    }


    /// Remap \p value from the range (0, \p max) to (0, \p max_out).
    ///
    /// \tparam Tp The output type.
    /// \tparam Up The input type.
    ///
    /// \param value The value to remap.
    /// \param max The maximum value of the input range.
    /// \param max_out The maximum value of the output range.
    ///
    /// \return The remapped value.
    ///
    /// \note This is a simplified version of remap() where the minimum values are 0.
    template<typename Tp, typename Up>
    constexpr auto remap_simple(Up value, Up max, Tp max_out) noexcept -> Tp
    {
        static_assert(std::is_arithmetic_v<Up>, "Type must be arithmetic");
        static_assert(std::is_arithmetic_v<Tp>, "Type must be arithmetic");

        return value * max_out / max;
    }

    /// @}
}  // namespace SenseShift