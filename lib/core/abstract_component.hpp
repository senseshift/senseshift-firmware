#pragma once

#include <set>
#include <type_traits>

namespace OH {
  class AbstractComponent {
   public:
    virtual void setup(void){};
    virtual void loop(void){};
  };

  template <class _Tp,
            typename std::enable_if<
                std::is_base_of<AbstractComponent, _Tp>::value>::type* =
                nullptr>
  class IComponentRegistry {
   public:
    virtual std::set<_Tp*> getComponents() = 0;
    virtual void registerComponent(_Tp*) = 0;
  };
}  // namespace OH
