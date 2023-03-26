#pragma once

#include <abstract_component.hpp>

namespace OH
{
  template <typename _Tp>
  class ISensor {
   public:
    virtual _Tp getValue() = 0;
  };

  template <typename _Tp>
  class RatePollingComponent : public Task<RatePollingComponent<_Tp>>, public ISensor<_Tp> {
    template<typename> friend class RatePollingComponent;

    protected:
      _Tp value;
      int rate;

    public:
      RatePollingComponent(TaskConfig taskConfig, int rate) : TaskComponent(taskConfig), rate(rate) {};
      void run(void) override {
        while (true) {
          this->value = this->updateValue();
          delay(this->rate);
        }
      };
      _Tp getValue() override { return this->value; };
      virtual _Tp updateValue(void) = 0;
  };
} // namespace OH
