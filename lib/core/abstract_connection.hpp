#pragma once

#include "abstract_component.hpp"
#include "events.hpp"
namespace OH {
  class AbstractConnection : public AbstractComponent, public IEventListener {
   public:
    virtual void handleEvent(const IEvent* event) const {};
  };
}  // namespace OH
