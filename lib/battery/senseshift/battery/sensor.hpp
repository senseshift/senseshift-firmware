#pragma once

#include <cstdint>

#include "senseshift/battery.hpp"

#include <senseshift/freertos/task.hpp>
#include <senseshift/input/sensor.hpp>
#include <senseshift/utility.hpp>

#ifndef SENSESHIFT_BATTERY_TASK_PRIORITY
#define SENSESHIFT_BATTERY_TASK_PRIORITY 1
#endif

namespace SenseShift::Battery {
    /**
     * Abstract battery sensor
     */
    using IBatterySensor = ::SenseShift::Input::ISimpleSensor<BatteryState>;

    class NaiveBatterySensor : public IBatterySensor {
      public:
        explicit NaiveBatterySensor(::SenseShift::Input::IFloatSensor* sensor) : sensor(sensor){};

        [[nodiscard]] auto getValue() -> BatteryState override
        {
            const auto level = static_cast<std::uint8_t>(this->sensor->getValue() * BatteryState::MAX_LEVEL);

            return { .level = level};
        };

        void init() override { this->sensor->init(); }

      private:
        ::SenseShift::Input::IFloatSensor* sensor;
    };

    class BatterySensor : public ::SenseShift::Input::MemoizedSensor<::SenseShift::Battery::BatteryState> {
      public:
        BatterySensor(::SenseShift::Battery::IBatterySensor* sensor, ::SenseShift::IEventDispatcher* eventDispatcher) :
          ::SenseShift::Input::MemoizedSensor<::SenseShift::Battery::BatteryState>(sensor),
          eventDispatcher(eventDispatcher){};

        void tick() override
        {
            this->::SenseShift::Input::MemoizedSensor<::SenseShift::Battery::BatteryState>::tick();
            this->eventDispatcher->postEvent(new ::SenseShift::Battery::BatteryLevelEvent(this->getValue()));
        }

      private:
        ::SenseShift::IEventDispatcher* eventDispatcher;
    };
} // namespace SenseShift::Battery
