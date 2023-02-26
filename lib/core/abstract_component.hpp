#pragma once

#include <set>
#include <type_traits>

#include "task.hpp"

#include <Arduino.h>

namespace OH {
  class IComponent {
   public:
    virtual void setup(void) {};
    virtual void begin(void) {};
  };

  class TaskComponent : public IComponent, public Task<TaskComponent> {
    friend class Task<TaskComponent>;

   public:
    TaskComponent(TaskConfig config) : Task<TaskComponent>(config) {};

    virtual void run() = 0;
  };

  class TaskLoopedComponent : public TaskComponent {
    friend class Task<TaskLoopedComponent>;
    friend class Task<TaskComponent>;
    friend class TaskComponent;

   public:
    TaskLoopedComponent(TaskConfig config) : TaskComponent(config) {};

    void run() override {
      while(1) {
        loop();
      }
    };

    virtual void loop(void) = 0;
  };

  class AbstractComponent : public IComponent {
  };

  template <class _Tp>
  class IComponentRegistry {
   public:
    virtual std::set<_Tp*> getComponents() = 0;
    virtual void registerComponent(_Tp*) = 0;
  };
}  // namespace OH
