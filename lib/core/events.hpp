#pragma once

#include <string>

namespace OH
{
  enum Event
  {

  };

  struct IEvent
  {
    const std::string eventName;

    IEvent(const std::string eventName) : eventName(eventName) {};
  };

  class IEventDispatcher
  {
   public:
    virtual void postEvent(const IEvent* event) = 0;
  };
} // namespace OH
