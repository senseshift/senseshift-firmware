#pragma once

#include "task.hpp"

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
    friend class Task<RatePollingComponent<_Tp>>;

    private:
      virtual void run(void) {
        while (true) {
          this->value = this->updateValue();
          delay(this->rate);
        }
      };

    protected:
      _Tp value;
      uint rate;
      virtual _Tp updateValue(void) = 0;

    public:
      RatePollingComponent(TaskConfig taskConfig, uint rate) : Task<RatePollingComponent<_Tp>>(taskConfig), rate(rate) {};
      _Tp getValue() override { return this->value; };
  };
} // namespace OH
