#pragma once

#include "senseshift/battery.hpp"

#include <senseshift/freertos/task.hpp>
#include <senseshift/input/sensor.hpp>
#include <senseshift/utility.hpp>

#ifndef SENSESHIFT_BATTERY_TASK_PRIORITY
#define SENSESHIFT_BATTERY_TASK_PRIORITY 1
#endif

namespace SenseShift::Battery {
    typedef struct BatteryConfig {
        uint sampleRate;
    } BatteryConfig_t;

    /**
     * Tasked sensor decorator
     */
    template<typename _Tp>
    class TaskedSensor :
      public ::SenseShift::FreeRTOS::Task<TaskedSensor<_Tp>>,
      public ::SenseShift::Input::MemoizedSensor<_Tp> {
        friend class ::SenseShift::FreeRTOS::Task<TaskedSensor<_Tp>>;

      public:
        TaskedSensor(
          ::SenseShift::Input::ISensor<_Tp>* sensor, ::SenseShift::FreeRTOS::TaskConfig taskConfig, const uint32_t rate
        ) :
          ::SenseShift::Input::MemoizedSensor<_Tp>(sensor),
          ::SenseShift::FreeRTOS::Task<TaskedSensor<_Tp>>(taskConfig),
          rate(rate){};

        void begin() override
        {
            this->setup();
            this->::SenseShift::FreeRTOS::Task<TaskedSensor<_Tp>>::begin();
        };

      protected:
        const uint32_t rate;

      private:
        virtual void run(void)
        {
            while (true) {
                this->updateValue();
                delay(this->rate);
            }
        };
    };

    /**
     * Abstract battery sensor
     */
    typedef ::SenseShift::Input::ISensor<BatteryState> IBatterySensor;

    class BatterySensor : public TaskedSensor<BatteryState> {
        friend class ::SenseShift::FreeRTOS::Task<TaskedSensor<BatteryState>>;
        friend class TaskedSensor<BatteryState>;

      public:
        BatterySensor(
          IBatterySensor* sensor,
          ::SenseShift::IEventDispatcher* eventDispatcher,
          const BatteryConfig_t& config,
          ::SenseShift::FreeRTOS::TaskConfig taskConfig
        ) :
          TaskedSensor<BatteryState>(sensor, taskConfig, config.sampleRate), eventDispatcher(eventDispatcher){};

        void run() override
        {
            while (true) {
                this->updateValue();
                this->eventDispatcher->postEvent(new BatteryLevelEvent(this->value));
                delay(this->rate);
            }
        }

      private:
        ::SenseShift::IEventDispatcher* eventDispatcher;
    };

    class NaiveBatterySensor : public IBatterySensor {
      public:
        NaiveBatterySensor(::SenseShift::Input::ISensor<uint16_t>* sensor) : sensor(sensor){};

        BatteryState getValue() override
        {
            return { .level =
                       static_cast<uint8_t>(::SenseShift::simpleMap<uint16_t>(this->sensor->getValue(), 4095, 255)) };
        };
        void setup() { this->sensor->setup(); }

      private:
        ::SenseShift::Input::ISensor<uint16_t>* sensor;
    };
} // namespace SenseShift::Battery
