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

  class TaskedComponent : public IComponent, public Task<TaskedComponent> {
    friend class Task<TaskedComponent>;

   public:
    TaskedComponent(TaskConfig config) : Task<TaskedComponent>(config) {};

    void run() override {
      while(1) {
        loop();
      }
    };

    virtual void loop(void) {
      if (this->getHandle() != nullptr) {
        vTaskDelete(NULL); // Do not loop if not implemented
      }
    };
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
