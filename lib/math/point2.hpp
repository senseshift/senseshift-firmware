#pragma once

#include <tuple>

namespace OH {
  template <typename _Tp>
  struct Point2 {
    _Tp x, y;

    Point2() : x((_Tp) 0), y((_Tp) 0) {};
    Point2(_Tp x, _Tp y) : x(x), y(y) {};
    Point2(const Point2<_Tp> &v) : x((_Tp) v.x), y((_Tp) v.y) {};

    bool operator==(const Point2 &rhs) const;
    bool operator!=(const Point2 &rhs) const;
    bool operator<(const Point2 &rhs) const;
  };

  typedef Point2<unsigned char> Point2b;
  typedef Point2<double> Point2d;
  typedef Point2<float> Point2f;
  typedef Point2<int> Point2i;
  typedef Point2<short> Point2s;
  typedef Point2<unsigned short> Point2w;

  template <typename _Tp> inline
  bool Point2<_Tp>::operator==(const Point2<_Tp> &rhs) const {
      return x == rhs.x && y == rhs.y;
  }

  template <typename _Tp> inline
  bool Point2<_Tp>::operator!=(const Point2<_Tp> &rhs) const {
      return !(*this == rhs);
  }

  template <typename _Tp> inline
  bool Point2<_Tp>::operator<(const Point2<_Tp> &rhs) const {
    return std::tie(x, y) < std::tie(rhs.x, rhs.y);
  }
}; // namespace OH
