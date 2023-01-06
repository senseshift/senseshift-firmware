#pragma once

#include <algorithm>
#include <iterator>

template<class C, typename T>
bool contains(C&& c, T t) { return std::find(std::begin(c), std::end(c), t) != std::end(c); };
