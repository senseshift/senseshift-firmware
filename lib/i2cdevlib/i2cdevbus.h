#ifndef SENSESHIFT_FIRMWARE_I2CDEVBUS_H
#define SENSESHIFT_FIRMWARE_I2CDEVBUS_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct i2cdev_bus_handle_t i2cdev_bus_handle_t;

typedef enum {
    I2CDEV_RESULT_OK = 0,
    I2CDEV_RESULT_ERROR = -1,
} i2cdev_result_t;

#ifdef __cplusplus
};
#endif

#endif // SENSESHIFT_FIRMWARE_I2CDEVBUS_H
