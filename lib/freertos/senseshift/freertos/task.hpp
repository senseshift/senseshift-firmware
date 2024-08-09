#pragma once

#include <cstdint>
#include <type_traits>

#include <senseshift/core/component.hpp>
#include <senseshift/core/logging.hpp>

extern "C" void delay(uint32_t ms);

#if defined(ESP32)
#include <Arduino.h>
#include <freertos/FreeRTOS.h> // Include the base FreeRTOS definitions.
#include <freertos/task.h>     // Include the task definitions.

extern "C" {
BaseType_t xTaskCreateUniversal(
  TaskFunction_t pxTaskCode,
  const char* const pcName,
  const uint32_t usStackDepth,
  void* const pvParameters,
  UBaseType_t uxPriority,
  TaskHandle_t* const pxCreatedTask,
  const BaseType_t xCoreID
);
}
#endif

namespace SenseShift::FreeRTOS {
struct TaskConfig {
    const char* name;
    uint32_t stackDepth;
    UBaseType_t priority;
    const BaseType_t coreId = tskNO_AFFINITY;
};

// Static polymorphic abstract base class for a FreeRTOS task using CRTP
// pattern. Concrete implementations should implement a run() method.
//
// Inspired by https://fjrg76.wordpress.com/2018/05/23/objectifying-task-creation-in-freertos-ii/
template<typename _Tp>
class Task {
    template<typename>
    friend class Task;

  public:
    explicit Task(const TaskConfig& config) : taskConfig(config)
    {
        log_i("creating Task: %s", taskConfig.name);
    }
    virtual ~Task()
    {
        if (taskHandle) {
            vTaskDelete(taskHandle);
        }
    }

    TaskHandle_t getHandle() const
    {
        return taskHandle;
    }

    virtual void begin()
    {
        BaseType_t result = xTaskCreateUniversal(
          taskFunction,                //< pvTaskCode
          this->taskConfig.name,       //< pcName
          this->taskConfig.stackDepth, //< usStackDepth
          this,                        //< pvParameters
          this->taskConfig.priority,   //< uxPriority
          &taskHandle,                 //< pvCreatedTask
          this->taskConfig.coreId      //< xCoreID
        );

        log_i("Created task %s, result %i", this->taskConfig.name, result);
        assert("Failed to create task" && result == pdPASS);
        if (!taskHandle) {
            log_e("Failed to create task %s", this->taskConfig.name);
        }
    }

  protected:
    inline void monitorTask()
    {
        log_i("----------------------------------------\nFree Heap: %d\n", xPortGetFreeHeapSize());
        volatile UBaseType_t uxArraySize = uxTaskGetNumberOfTasks();
        TaskStatus_t* pxTaskStatusArray = (TaskStatus_t*) pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));
        if (!pxTaskStatusArray) {
            log_e("Failed to allocate memory for task status array!");
            return;
        }
        uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, NULL);
        for (UBaseType_t i = 0; i < uxArraySize; i++) {
            log_i(
              "Task: %s\n\tStack High Watermark: %d\n\tState: %d\n",
              pxTaskStatusArray[i].pcTaskName,
              pxTaskStatusArray[i].usStackHighWaterMark,
              pxTaskStatusArray[i].eCurrentState
            );

            if (pxTaskStatusArray[i].usStackHighWaterMark < 20) {
                log_w(
                  "Warning: Task %s has low stack space, only %dB awailable!",
                  pxTaskStatusArray[i].pcTaskName,
                  pxTaskStatusArray[i].usStackHighWaterMark * 4
                );
            }
        }
        vPortFree(pxTaskStatusArray);
    }

  private:
    const TaskConfig& taskConfig;
    TaskHandle_t taskHandle = nullptr;

    static void taskFunction(void* params)
    {
        _Tp* task = static_cast<_Tp*>(params);
        task->run();
    }
};

template<typename Tp>
class ComponentUpdateTask : public Task<ComponentUpdateTask<Tp>> {
    static_assert(std::is_same_v<decltype(&Tp::init), void (Tp::*)()>);
    static_assert(std::is_same_v<decltype(&Tp::tick), void (Tp::*)()>);

  public:
    ComponentUpdateTask(Tp* component, std::uint32_t updateDelay, const TaskConfig& taskConfig) :
      Task<ComponentUpdateTask<Tp>>(taskConfig), component_(component), updateDelay_(updateDelay)
    {
        log_i("creating ComponentUpdateTask: %s", taskConfig.name);
    }

    void begin() override
    {
        this->component_->init();
        this->Task<ComponentUpdateTask>::begin();
    }

  protected:
    [[noreturn]] void run()
    {
        auto now = millis();

        while (true) {
            now = millis();

            this->component_->tick();

            const auto elapsed = millis() - now;

            log_d("T: %d, Fmax: %dHz, Ft: %dHz", elapsed, 1000 / elapsed, 1000 / this->updateDelay_);
            if (elapsed < this->updateDelay_) {
                delay(this->updateDelay_ - elapsed);
            }
        }
    }

  private:
    friend class Task<ComponentUpdateTask>;

    Tp* component_;
    std::uint32_t updateDelay_;
};
} // namespace SenseShift::FreeRTOS
