#include "vec2.hpp"

#include <tuple>

// https://stackoverflow.com/questions/8752837
template class OH::Math::Vec2<unsigned char>;
template class OH::Math::Vec2<double>;
template class OH::Math::Vec2<float>;
template class OH::Math::Vec2<int>;
template class OH::Math::Vec2<short>;
template class OH::Math::Vec2<unsigned short>;

template <typename _Tp>
bool OH::Math::Vec2<_Tp>::operator==(const OH::Math::Vec2<_Tp> &rhs) const {
    return x == rhs.x && y == rhs.y;
}

template <typename _Tp>
bool OH::Math::Vec2<_Tp>::operator!=(const OH::Math::Vec2<_Tp> &rhs) const {
    return !(*this == rhs);
}

template <typename _Tp>
bool OH::Math::Vec2<_Tp>::operator<(const OH::Math::Vec2<_Tp> &rhs) const {
  return std::tie(x, y) < std::tie(rhs.x, rhs.y);
}
