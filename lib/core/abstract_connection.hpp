#pragma once

#include "abstract_component.hpp"
#include "events.hpp"
namespace OH {
  class AbstractConnection : public AbstractComponent, public IEventListener {
   public:
    AbstractConnection(TaskConfig config) : AbstractComponent(config) {};
    virtual void handleEvent(const IEvent* event) const {};
  };
}  // namespace OH
