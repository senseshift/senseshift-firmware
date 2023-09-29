#pragma once

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
        NaiveBatterySensor(::SenseShift::Input::ISimpleSensor<uint16_t>* sensor) : sensor(sensor){};

        BatteryState getValue() override
        {
            return { .level =
                       static_cast<uint8_t>(::SenseShift::simpleMap<uint16_t>(this->sensor->getValue(), 4095, 255)) };
        };
        void init() override { this->sensor->init(); }

      private:
        ::SenseShift::Input::ISimpleSensor<uint16_t>* sensor;
    };

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
} // namespace SenseShift::Battery
