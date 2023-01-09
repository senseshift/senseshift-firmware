#pragma once

#include "esp_log.h"

#define OH_LOGE(tag, format, ...) ESP_LOGE(tag, format, ##__VA_ARGS__)
#define OH_LOGW(tag, format, ...) ESP_LOGW(tag, format, ##__VA_ARGS__)
#define OH_LOGI(tag, format, ...) ESP_LOGI(tag, format, ##__VA_ARGS__)
#define OH_LOGD(tag, format, ...) ESP_LOGD(tag, format, ##__VA_ARGS__)
#define OH_LOGV(tag, format, ...) ESP_LOGV(tag, format, ##__VA_ARGS__)
