#include "vec2.hpp"

#include <tuple>

// https://stackoverflow.com/questions/8752837
template class OH::Vec2<unsigned char>;
template class OH::Vec2<double>;
template class OH::Vec2<float>;
template class OH::Vec2<int>;
template class OH::Vec2<short>;
template class OH::Vec2<unsigned short>;

template <typename _Tp>
bool OH::Vec2<_Tp>::operator==(const OH::Vec2<_Tp> &rhs) const {
    return x == rhs.x && y == rhs.y;
}

template <typename _Tp>
bool OH::Vec2<_Tp>::operator!=(const OH::Vec2<_Tp> &rhs) const {
    return !(*this == rhs);
}

template <typename _Tp>
bool OH::Vec2<_Tp>::operator<(const OH::Vec2<_Tp> &rhs) const {
  return std::tie(x, y) < std::tie(rhs.x, rhs.y);
}
