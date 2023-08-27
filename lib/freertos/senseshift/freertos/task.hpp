#pragma once

#include <senseshift/logging.hpp>

extern "C" void delay(uint32_t ms);

#if defined(ESP32)
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
        Task(const char* name, uint32_t stackDepth, UBaseType_t priority, const BaseType_t coreId = tskNO_AFFINITY)
        {
            this->taskConfig = { name, stackDepth, priority, coreId };
        };
        Task(TaskConfig& config) : taskConfig(config){};
        virtual ~Task()
        {
            if (taskHandle) {
                vTaskDelete(taskHandle);
            }
        };

        TaskHandle_t getHandle() const { return taskHandle; };

        virtual void begin()
        {
            BaseType_t result = xTaskCreateUniversal(
              taskFunction,                // pvTaskCode
              this->taskConfig.name,       // pcName
              this->taskConfig.stackDepth, // usStackDepth
              this,                        // pvParameters
              this->taskConfig.priority,   // uxPriority
              &taskHandle,                 // pvCreatedTask
              this->taskConfig.coreId      // xCoreID
            );

            assert("Failed to create task" && result == pdPASS);
            if (!taskHandle) {
                log_e("Failed to create task %s", this->taskConfig.name);
            }
        };

      private:
        const TaskConfig& taskConfig;
        TaskHandle_t taskHandle = nullptr;

        static void taskFunction(void* params)
        {
            _Tp* task = static_cast<_Tp*>(params);
            task->run();
        }
    };

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
} // namespace SenseShift::FreeRTOS