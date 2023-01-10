#pragma once

#include "abstract_component.hpp"

#include <vector>

namespace OH {
  template <class _Tp,
            typename std::enable_if<
                std::is_base_of<AbstractComponent, _Tp>::value>::type* =
                nullptr>
  class IComponentRegistry {
   public:
    virtual std::vector<_Tp*> getComponents() = 0;
    virtual void registerComponent(_Tp*) = 0;
  };
}  // namespace OH
