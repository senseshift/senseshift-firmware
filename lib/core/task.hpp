#pragma once

#include "logging.hpp"

extern "C" void delay(uint32_t ms);

#if defined(ARDUINO_ARCH_ESP32)
#include <freertos/FreeRTOS.h>   // Include the base FreeRTOS definitions.
#include <freertos/task.h>       // Include the task definitions.

extern "C" BaseType_t xTaskCreateUniversal(
  TaskFunction_t pxTaskCode,
  const char * const pcName,
  const uint32_t usStackDepth,
  void * const pvParameters,
  UBaseType_t uxPriority,
  TaskHandle_t * const pxCreatedTask,
  const BaseType_t xCoreID
);
#endif

namespace OH {
  struct TaskConfig {
    const char *name;
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
   template<typename> friend class Task;

   private:
    TaskConfig taskConfig;
    TaskHandle_t taskHandle = nullptr;

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
    virtual ~Task() {};

    TaskHandle_t getHandle() const { return taskHandle; };

    virtual void begin() {
      BaseType_t result = xTaskCreateUniversal(
        taskFunction,                 // pvTaskCode
        this->taskConfig.name,        // pcName
        this->taskConfig.stackDepth,  // usStackDepth
        this,                         // pvParameters
        this->taskConfig.priority,    // uxPriority
        &taskHandle,                  // pvCreatedTask
        this->taskConfig.coreId       // xCoreID
      );

      assert("Failed to create task" && result == pdPASS);
      if(!taskHandle) {
        log_e("Failed to create task %s", this->taskConfig.name);
      }
    };
  };
}
