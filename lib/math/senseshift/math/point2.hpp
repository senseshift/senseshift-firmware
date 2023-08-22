#pragma once

#include <cmath>
#include <tuple>

namespace SenseShift::Math {
    template<typename _Tp>
    struct Point2 {
        static_assert(
          std::is_arithmetic<_Tp>::value, "::SenseShift::Math::Point2 only can be used with arithmetic types"
        );

        typedef _Tp Value_t;

        inline static const _Tp MIN = std::numeric_limits<_Tp>::min();
        inline static const _Tp MAX = std::numeric_limits<_Tp>::max();

        _Tp x, y;

        constexpr Point2() : x((_Tp) 0), y((_Tp) 0){};
        constexpr Point2(_Tp x, _Tp y) : x(x), y(y){};
        constexpr Point2(const Point2<_Tp>& v) : x((_Tp) v.x), y((_Tp) v.y){};

        constexpr inline bool operator==(const Point2<_Tp>& rhs) const { return x == rhs.x && y == rhs.y; }

        constexpr inline bool operator!=(const Point2<_Tp>& rhs) const { return !(*this == rhs); }

        constexpr bool operator<(const Point2<_Tp>& rhs) const { return std::tie(x, y) < std::tie(rhs.x, rhs.y); }

        constexpr float operator-(const Point2<_Tp>& rhs) const
        {
            return std::sqrt(std::pow(x - rhs.x, 2) + std::pow(y - rhs.y, 2));
        }
    };

    typedef Point2<unsigned char> Point2b;
    typedef Point2<double> Point2d;
    typedef Point2<float> Point2f;
    typedef Point2<int> Point2i;
    typedef Point2<short> Point2s;
    typedef Point2<unsigned short> Point2w;
}; // namespace SenseShift::Math
