#pragma once

#include <set>
#include <type_traits>

#include "task.hpp"

#include <Arduino.h>

namespace OH {
  class IComponent {
   public:
    virtual void setup(void) {};
  };

  template <class _Tp>
  class IComponentRegistry {
   public:
    virtual std::set<_Tp*> getComponents() = 0;
    virtual void registerComponent(_Tp*) = 0;
  };
}  // namespace OH
