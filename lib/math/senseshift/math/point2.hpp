#pragma once

#include <cmath>
#include <limits>
#include <tuple>
#include <type_traits>

namespace SenseShift::Math {
    template<typename Tp>
    struct Point2 {
        static_assert(std::is_arithmetic_v<Tp>, "Point2 only can be used with arithmetic types");

        using Value = Tp;

        static constexpr Tp MIN = std::numeric_limits<Tp>::min();
        static constexpr Tp MAX = std::numeric_limits<Tp>::max();

        Tp x, y;

        constexpr Point2() : x(static_cast<Tp>(0)), y(static_cast<Tp>(0)){};
        constexpr Point2(Tp x, Tp y) : x(x), y(y){};
        constexpr Point2(const Point2<Tp>& v) : x((Tp) v.x), y((Tp) v.y){};

        constexpr inline auto operator==(const Point2<Tp>& rhs) const -> bool { return x == rhs.x && y == rhs.y; }

        constexpr inline auto operator!=(const Point2<Tp>& rhs) const -> bool { return !(*this == rhs); }

        constexpr auto operator<(const Point2<Tp>& rhs) const -> bool { return std::tie(x, y) < std::tie(rhs.x, rhs.y); }

        constexpr auto operator-(const Point2<Tp>& rhs) const -> float
        {
            return std::sqrt(std::pow(x - rhs.x, 2) + std::pow(y - rhs.y, 2));
        }
    };

    using Point2b = Point2<unsigned char>;
    using Point2d = Point2<double>;
    using Point2f = Point2<float>;
    using Point2i = Point2<int>;
    using Point2s = Point2<short>;
    using Point2w = Point2<unsigned short>;
}; // namespace SenseShift::Math
