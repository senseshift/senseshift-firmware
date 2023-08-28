#pragma once

#include <senseshift/battery/sensor.hpp>
#include <senseshift/freertos/input/sensor.hpp>

namespace SenseShift::FreeRTOS::Battery {
    class BatterySensor : public ::SenseShift::Input::MemoizedSensor<::SenseShift::Battery::BatteryState> {
      public:
        BatterySensor(::SenseShift::Battery::IBatterySensor* sensor, ::SenseShift::IEventDispatcher* eventDispatcher) :
          ::SenseShift::Input::MemoizedSensor<::SenseShift::Battery::BatteryState>(sensor),
          eventDispatcher(eventDispatcher){};

        void tick() override
        {
            this->::SenseShift::Input::MemoizedSensor<::SenseShift::Battery::BatteryState>::tick();
            this->eventDispatcher->postEvent(new ::SenseShift::Battery::BatteryLevelEvent(this->value));
        }

      private:
        ::SenseShift::IEventDispatcher* eventDispatcher;
    };
} // namespace SenseShift::FreeRTOS::Battery
