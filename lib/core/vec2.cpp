#include "vec2.hpp"

#include <tuple>

template <typename T>
bool OH::Math::Vec2<T>::operator==(const OH::Math::Vec2<T> &rhs) const {
    return x == rhs.x && y == rhs.y;
}

template <typename T>
bool OH::Math::Vec2<T>::operator!=(const OH::Math::Vec2<T> &rhs) const {
    return !(*this == rhs);
}

template <typename T>
bool OH::Math::Vec2<T>::operator<(const OH::Math::Vec2<T> &rhs) const {
  return std::tie(x, y) < std::tie(rhs.x, rhs.y);
}
