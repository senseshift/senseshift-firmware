#pragma once

#include "abstract_component.hpp"

namespace OH {
  class AbstractConnection : public AbstractComponent {
   public:
    virtual void setup(void){};
    virtual void loop(void){};
  };
}  // namespace OH
