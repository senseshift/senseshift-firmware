#pragma once

#if defined(ARDUINO_ARCH_ESP32)
  #include <esp32-hal-log.h>
#elif defined(UNITY_INCLUDE_PRINT_FORMATTED)
  #define log_e(...) TEST_PRINTF(__VA_ARGS__)
  #define log_w(...) TEST_PRINTF(__VA_ARGS__)
  #define log_i(...) TEST_PRINTF(__VA_ARGS__)
  #define log_d(...) TEST_PRINTF(__VA_ARGS__)
  #define log_t(...) TEST_PRINTF(__VA_ARGS__)
#else
  #define log_e(...)
  #define log_w(...)
  #define log_i(...)
  #define log_d(...)
  #define log_t(...)
#endif
