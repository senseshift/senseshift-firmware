#pragma once

#include <events.hpp>
#include <sensor.hpp>
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
    class TaskedSensor : public Task<TaskedSensor<_Tp>>, public MemoizedSensor<_Tp> {
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
        TaskedSensor(ISensor<_Tp>* sensor, TaskConfig taskConfig, uint32_t rate) :
          MemoizedSensor<_Tp>(sensor), Task<TaskedSensor<_Tp>>(taskConfig), rate(rate){};

        void begin() override
        {
            this->setup();
            this->Task<TaskedSensor<_Tp>>::begin();
        };
    };

    struct BatteryState {
        uint8_t level;
    };

    class BatteryLevelEvent : public IEvent {
      public:
        const BatteryState state;
        BatteryLevelEvent(const BatteryState state) : IEvent(OH_EVENT_BATTERY_LEVEL), state(state){};
    };

    struct BatteryConfig {
        uint sampleRate;
    };

    /**
     * Abstract battery sensor
     */
    typedef ISensor<BatteryState> IBatterySensor;

    class BatterySensor : public TaskedSensor<BatteryState> {
        friend class Task<TaskedSensor<BatteryState>>;
        friend class TaskedSensor<BatteryState>;

      private:
        IEventDispatcher* eventDispatcher;

      public:
        BatterySensor(
          IBatterySensor* sensor, IEventDispatcher* eventDispatcher, BatteryConfig config, TaskConfig taskConfig
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
