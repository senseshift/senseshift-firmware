#pragma once

#include <string>

namespace OH
{
  enum Event
  {

  };

  struct IEvent
  {
    const std::string event;

    IEvent(const std::string event) : event(event) {};
  };

  class IEventDispatcher
  {
    virtual void postEvent(const IEvent* event) = 0;
  };
} // namespace OH
