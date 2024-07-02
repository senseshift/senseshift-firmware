#ifndef SENSESHIFT_FIRMWARE_I2CDEVBUS_H
#define SENSESHIFT_FIRMWARE_I2CDEVBUS_H

#include "stdint.h"

/// 1000ms default read timeout (modify with `I2CDevBus::DEFAULT_READ_TIMEOUT_MS = [ms];`)
#define I2CDEV_DEFAULT_READ_TIMEOUT_MS 1000

#ifdef __cplusplus
extern "C" {
#endif

typedef struct i2cdev_bus_handle_t i2cdev_bus_handle_t;

typedef enum {
    I2CDEV_RESULT_OK = 0,
    I2CDEV_RESULT_ERROR = -1,
    I2CDEV_RESULT_WOULD_BLOCK = -2,
} i2cdev_result_t;

typedef uint8_t i2cdev_dev_addr_t;
typedef uint8_t i2cdev_reg_addr_t;

#ifdef __cplusplus
};
#endif

#endif // SENSESHIFT_FIRMWARE_I2CDEVBUS_H
