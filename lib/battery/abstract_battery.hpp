#pragma once

#include <senseshift/events.hpp>
#include <senseshift/input/sensor.hpp>
#include <task.hpp>

#include <stdint.h>

#ifndef BATTERY_TASK_PRIORITY
#define BATTERY_TASK_PRIORITY 1
#endif

namespace OH {
    /**
     * Tasked sensor decorator
     */
    template<typename _Tp>
    class TaskedSensor : public Task<TaskedSensor<_Tp>>, public ::SenseShift::Input::MemoizedSensor<_Tp> {
        friend class Task<TaskedSensor<_Tp>>;

      private:
        virtual void run(void)
        {
            while (true) {
                this->updateValue();
                delay(this->rate);
            }
        };

      protected:
        uint32_t rate;

      public:
        TaskedSensor(::SenseShift::Input::ISensor<_Tp>* sensor, TaskConfig taskConfig, uint32_t rate) :
          ::SenseShift::Input::MemoizedSensor<_Tp>(sensor), Task<TaskedSensor<_Tp>>(taskConfig), rate(rate){};

        void begin() override
        {
            this->setup();
            this->Task<TaskedSensor<_Tp>>::begin();
        };
    };

    struct BatteryState {
        uint8_t level;
    };

    class BatteryLevelEvent : public ::SenseShift::IEvent {
      public:
        const BatteryState state;
        BatteryLevelEvent(const BatteryState state) : ::SenseShift::IEvent(OH_EVENT_BATTERY_LEVEL), state(state){};
    };

    struct BatteryConfig {
        uint sampleRate;
    };

    /**
     * Abstract battery sensor
     */
    typedef ::SenseShift::Input::ISensor<BatteryState> IBatterySensor;

    class BatterySensor : public TaskedSensor<BatteryState> {
        friend class Task<TaskedSensor<BatteryState>>;
        friend class TaskedSensor<BatteryState>;

      private:
        ::SenseShift::IEventDispatcher* eventDispatcher;

      public:
        BatterySensor(
          IBatterySensor* sensor,
          ::SenseShift::IEventDispatcher* eventDispatcher,
          BatteryConfig config,
          TaskConfig taskConfig
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
    };
} // namespace OH
