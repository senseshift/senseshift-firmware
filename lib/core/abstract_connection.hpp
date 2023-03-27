#pragma once

#include "abstract_component.hpp"
#include "events.hpp"
namespace OH {
  class AbstractConnection {
   public:
    virtual void begin(void) {};
  };
}  // namespace OH
