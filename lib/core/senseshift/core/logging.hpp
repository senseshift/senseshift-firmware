#pragma once

#if defined(ESP32)

#include <esp32-hal-log.h>
#define LOG_E(tag, format, ...) ESP_LOGE(tag, format, ##__VA_ARGS__)
#define LOG_W(tag, format, ...) ESP_LOGW(tag, format, ##__VA_ARGS__)
#define LOG_I(tag, format, ...) ESP_LOGI(tag, format, ##__VA_ARGS__)
#define LOG_D(tag, format, ...) ESP_LOGD(tag, format, ##__VA_ARGS__)
#define LOG_V(tag, format, ...) ESP_LOGV(tag, format, ##__VA_ARGS__)

#elif defined(UNITY_INCLUDE_PRINT_FORMATTED)

#define log_e(...) TEST_PRINTF(__VA_ARGS__)
#define log_w(...) TEST_PRINTF(__VA_ARGS__)
#define log_i(...) TEST_PRINTF(__VA_ARGS__)
#define log_d(...) TEST_PRINTF(__VA_ARGS__)
#define log_v(...) TEST_PRINTF(__VA_ARGS__)

#define LOG_E(tag, format, ...) log_e("[%s] " format, tag, ##__VA_ARGS__)
#define LOG_W(tag, format, ...) log_w("[%s] " format, tag, ##__VA_ARGS__)
#define LOG_I(tag, format, ...) log_i("[%s] " format, tag, ##__VA_ARGS__)
#define LOG_D(tag, format, ...) log_d("[%s] " format, tag, ##__VA_ARGS__)
#define LOG_V(tag, format, ...) log_v("[%s] " format, tag, ##__VA_ARGS__)

#else

#define log_e(...)
#define log_w(...)
#define log_i(...)
#define log_d(...)
#define log_v(...)

#define LOG_E(tag, format, ...)
#define LOG_W(tag, format, ...)
#define LOG_I(tag, format, ...)
#define LOG_D(tag, format, ...)
#define LOG_V(tag, format, ...)

#endif
