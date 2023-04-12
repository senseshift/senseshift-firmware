#pragma once

#if defined(ARDUINO_ARCH_ESP32)
  #include <esp32-hal-log.h>
#else
  #define log_e(...)
  #define log_w(...)
  #define log_i(...)
  #define log_d(...)
  #define log_t(...)
#endif
