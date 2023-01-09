#pragma once

namespace OH {
  namespace Math {
    template <typename _Tp>
    struct Vec2 {
      _Tp x, y;

      Vec2() : x((_Tp) 0), y((_Tp) 0) {};
      Vec2(_Tp x, _Tp y) : x(x), y(y) {};
      Vec2(const Vec2<_Tp> &v) : x((_Tp) v.x), y((_Tp) v.y) {};

      bool operator==(const Vec2 &rhs) const;
      bool operator!=(const Vec2 &rhs) const;
      bool operator<(const Vec2 &rhs) const;
    };

    typedef Vec2<unsigned char> Vec2b;
    typedef Vec2<double> Vec2d;
    typedef Vec2<float> Vec2f;
    typedef Vec2<int> Vec2i;
    typedef Vec2<short> Vec2s;
    typedef Vec2<unsigned short> Vec2w;
  }
}
