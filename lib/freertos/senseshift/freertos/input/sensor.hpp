#pragma once

#include <senseshift/freertos/task.hpp>
#include <senseshift/input/sensor.hpp>

#include <cstdint>

namespace SenseShift::FreeRTOS::Input {
    /// Sensor update task
    template<typename Sensor>
    class SensorUpdateTask : public Task<SensorUpdateTask<Sensor>> {
        static_assert(std::is_same_v<decltype(&Sensor::init), void (Sensor::*)()>);
        static_assert(std::is_same_v<decltype(&Sensor::tick), void (Sensor::*)()>);

      public:
        SensorUpdateTask(Sensor* sensor, std::uint32_t updateDelay, TaskConfig taskConfig) :
          Task<SensorUpdateTask>(taskConfig), sensor_(sensor), updateDelay_(updateDelay) {
            log_i("creating SensorUpdateTask: %s", taskConfig.name);
        };

        void begin() override {
            this->sensor_->init();
            this->Task<SensorUpdateTask>::begin();
        }

      protected:
        [[noreturn]] void run()
        {
            while (true) {
                this->sensor_->tick();
                delay(this->updateDelay_);
                // log_i("high watermark %d", uxTaskGetStackHighWaterMark(NULL));
            }
        }

      private:
        friend class Task<SensorUpdateTask>;

        Sensor* sensor_;
        std::uint32_t updateDelay_;
    };
} // namespace SenseShift::FreeRTOS::Input
