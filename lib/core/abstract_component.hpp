#pragma once

#include <set>
#include <type_traits>

#include "task.hpp"

namespace OH {

  class AbstractComponent : public Task<AbstractComponent> {
   friend class Task<AbstractComponent>;

   public:
    AbstractComponent(TaskConfig config) : Task<AbstractComponent>(config) {};

    void run() override {
      setup();

      while(1) {
        loop();
      }
    };

    virtual void setup(void){};
    virtual void loop(void) {
      vTaskDelete(NULL); // Do not loop if not implemented
    };
  };

  template <class _Tp>
  class IComponentRegistry {
   public:
    virtual std::set<_Tp*> getComponents() = 0;
    virtual void registerComponent(_Tp*) = 0;
  };
}  // namespace OH
