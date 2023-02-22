#pragma once

#include <freertos/FreeRTOS.h>   // Include the base FreeRTOS definitions.
#include <freertos/task.h>       // Include the task definitions.

namespace OH {
  struct TaskConfig {
    const char *name;
    uint32_t stackDepth;
    UBaseType_t priority;
    const BaseType_t coreId = tskNO_AFFINITY;
  };

  template<class _Tp>
  class Task {
   private:
    TaskConfig taskConfig;
    TaskHandle_t taskHandle;

    static void taskFunction(void *params) {
      _Tp* task = static_cast<_Tp*>(params);
      task->run();
    }

   public:
    Task(const char *name, uint32_t stackDepth, UBaseType_t priority, const BaseType_t coreId = tskNO_AFFINITY) {
      TaskConfig config = {name, stackDepth, priority, coreId};

      this->taskConfig = config;
    };
    Task(TaskConfig config) : taskConfig(config) {};

    TaskHandle_t getHandle() { return taskHandle; };

    void begin() {
      BaseType_t result = xTaskCreatePinnedToCore(
        taskFunction,
        this->taskConfig->name,
        this->taskConfig->stackDepth,
        this,
        this->taskConfig->priority,
        &taskHandle,
        this->taskConfig->coreId
      );
      assert("Failed to create task" && result == pdPASS);
    };

    virtual void run() = 0;
  };
}
