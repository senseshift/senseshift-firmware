#pragma once

#include "logging.hpp"

#include <Arduino.h>

#if defined(ARDUINO_ARCH_ESP32)
  #include <freertos/FreeRTOS.h>   // Include the base FreeRTOS definitions.
  #include <freertos/task.h>       // Include the task definitions.
#elif defined(ARDUINO_ARCH_STM32)
  #include <STM32FreeRTOS.h>

  #define tskNO_AFFINITY 0
  #define xTaskCreateUniversal(pxTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask, xCoreID) xTaskCreate(pxTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask)
#endif

namespace OH {
  struct TaskConfig {
    const char *name;
    uint32_t stackDepth;
    UBaseType_t priority;
    const BaseType_t coreId = tskNO_AFFINITY;
  };

  class TaskComponent;

  // Static polymorphic abstract base class for a FreeRTOS task using CRTP
  // pattern. Concrete implementations should implement a run() method.
  //
  // Inspired by https://fjrg76.wordpress.com/2018/05/23/objectifying-task-creation-in-freertos-ii/
  template<typename _Tp>
  class Task {
   template<typename> friend class Task;
   friend class TaskComponent;

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

      // assert(result == pdPASS);
      if(!taskHandle || !result) {
        log_e("Failed to create task %s", this->taskConfig.name);
      }
    };
  };
}
