#pragma once

#if defined(ARDUINO_ARCH_ESP32)
  #include <esp32-hal-log.h>
#elif defined(ARDUINO_ARCH_STM32)
  #define log_e(...) ((void)0)
  #define log_w(...) ((void)0)
  #define log_i(...) ((void)0)
  #define log_d(...) ((void)0)
  #define log_t(...) ((void)0)
#endif
