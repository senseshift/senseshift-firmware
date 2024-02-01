#pragma once

#include <functional>
#include <type_traits>
#include <vector>

namespace SenseShift {
    /// Linearly interpolate between \p start and \p end by \p completion (between 0 and 1).
    template<typename Tp>
    constexpr auto lerp(const float completion, const Tp start, const Tp end) -> Tp
    {
        static_assert(std::is_arithmetic_v<Tp>, "Type must be arithmetic");

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
        static_assert(std::is_arithmetic_v<Tp>, "Type must be arithmetic");
        static_assert(std::is_arithmetic_v<Up>, "Type must be arithmetic");

        if (max <= min) {
            LOG_E("util.remap", "Invalid input range, min <= max");
            return (min_out + max_out) / 2;
        }

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

    /// Lookup a value in a table and interpolate between the two closest values.
    ///
    /// \tparam Tp
    /// \tparam To
    /// \tparam Container
    ///
    /// \param lookup_table Lookup table to use in the format of std::map<Tp, Tp> in descending order.
    /// \param value
    ///
    /// \return
    template <typename Tp, typename To, typename Container>
    auto lookup_interpolate(Container const& lookup_table, Tp value) -> To
    {
        static_assert(std::is_same_v<typename Container::key_type, Tp>);
        static_assert(std::is_same_v<typename Container::mapped_type, To>);
        static_assert(std::is_arithmetic_v<Tp>, "lookup_interpolate only supports arithmetic types");
        static_assert(std::is_arithmetic_v<To>, "lookup_interpolate only supports arithmetic types");

        // If the value is outside the range of the lookup table, return the closest value
        if (value <= lookup_table.begin()->first) {
            return lookup_table.begin()->second;
        }
        if (value >= lookup_table.rbegin()->first) {
            return lookup_table.rbegin()->second;
        }

        // Find the two closest values in the lookup table
        auto const upper = lookup_table.lower_bound(value);
        auto const lower = std::prev(upper);

        // Interpolate between the two closest values
        float const completion = (value - lower->first) / (upper->first - lower->first);

        return lerp(completion, lower->second, upper->second);
    }

    template<typename... X>
    class CallbackManager;

     /// Helper class to allow having multiple subscribers to a callback.
     ///
     /// \tparam Ts The arguments for the callbacks, wrapped in void().
    template<typename... Ts>
    class CallbackManager<void(Ts...)> {
      public:
        using CallbackType = std::function<void(Ts...)>;

        /// Add a callback to the list.
        void add(std::function<void(Ts...)> &&callback) { this->callbacks_.push_back(std::move(callback)); }

        /// Call all callbacks in this manager.
        void call(Ts... args) {
            for (auto &callback : this->callbacks_) {
                callback(args...);
            }
        }
        [[nodiscard]] auto size() const -> size_t { return this->callbacks_.size(); }

        /// Call all callbacks in this manager.
        void operator()(Ts... args) { call(args...); }

      private:
        std::vector<std::function<void(Ts...)>> callbacks_;
    };
}  // namespace SenseShift
