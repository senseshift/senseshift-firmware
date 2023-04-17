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
  class ThrottledSensor : public Task<ThrottledSensor<_Tp>>, public ISensor<_Tp> {
    template<typename> friend class RatePollingComponent;
    friend class Task<ThrottledSensor<_Tp>>;

    private:
      virtual void run(void) {
        while (true) {
          this->value = this->updateValue();
          delay(this->rate);
        }
      };

    protected:
      _Tp value;
      uint32_t rate;
      virtual _Tp updateValue(void) = 0;

    public:
      ThrottledSensor(TaskConfig taskConfig, uint32_t rate) : Task<ThrottledSensor<_Tp>>(taskConfig), rate(rate) {};
      _Tp getValue() override { return this->value; };
  };
} // namespace OH
