#pragma once

#include <vector>

namespace OH
{
  template <class _Tp>
  class IComponentRegistry
  {
   public:
    virtual std::vector<_Tp*> getComponents() = 0;
    virtual void registerComponent(_Tp*) = 0;
  };
} // namespace OH
