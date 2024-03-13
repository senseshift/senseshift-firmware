#pragma once

#include <cstddef>
#include <cstdint>

#include "i2cdevbus.hpp"

#ifdef ARDUINO

#include <Wire.h>

class ArduinoI2CDev : public I2CDevBus {
  public:
    explicit ArduinoI2CDev(TwoWire* wire = &Wire) : wire_(wire) {}

  private:
    TwoWire* wire_;

    auto readReg8(
      std::uint8_t devAddr,
      std::uint8_t regAddr,
      std::size_t length,
      std::uint8_t* data,
      std::uint16_t timeout = DEFAULT_READ_TIMEOUT_MS
    ) -> i2cdev_result_t override
    {
        this->wire_->beginTransmission(devAddr);
        this->wire_->write(regAddr);
        this->wire_->endTransmission();

        this->wire_->requestFrom(devAddr, length, true);

        std::uint8_t received = 0;
        const auto start = millis();

        while (this->wire_->available() && (timeout == 0 || millis() - start < timeout)) {
            data[received++] = this->wire_->read();

            // Return if we have received all the bytes we need
            if (received == length) {
                return I2CDEV_RESULT_OK;
            }
        }

        if (timeout > 0 && millis() - start >= timeout && received < length) {
            return I2CDEV_RESULT_ERROR;
        }

        return I2CDEV_RESULT_OK;
    }

    auto readReg16(
      std::uint8_t devAddr,
      std::uint8_t regAddr,
      std::size_t length,
      std::uint16_t* data,
      std::uint16_t timeout = DEFAULT_READ_TIMEOUT_MS
    ) -> i2cdev_result_t override
    {
        this->wire_->beginTransmission(devAddr);
        this->wire_->write(regAddr);
        this->wire_->endTransmission();

        this->wire_->requestFrom(devAddr, length * 2, true);

        std::uint8_t received = 0;
        const auto start = millis();

        while (this->wire_->available() >= 2 && (timeout == 0 || millis() - start < timeout)) {
            data[received++] = this->wire_->read() << 8 | this->wire_->read();

            // Return if we have received all the bytes we need
            if (received == length * 2) {
                return I2CDEV_RESULT_OK;
            }
        }

        if (timeout > 0 && millis() - start >= timeout && received < length * 2) {
            return I2CDEV_RESULT_ERROR;
        }

        return I2CDEV_RESULT_OK;
    }

    auto writeReg8(std::uint8_t devAddr, std::uint8_t regAddr, std::size_t length, const std::uint8_t* data)
      -> i2cdev_result_t override
    {
        this->wire_->beginTransmission(devAddr);

        // Send address
        if (this->wire_->write(regAddr) != 1) {
            return I2CDEV_RESULT_ERROR;
        }

        // Send data
        if (this->wire_->write(data, length) != length) {
            return I2CDEV_RESULT_ERROR;
        }

        if (this->wire_->endTransmission() != 0) {
            return I2CDEV_RESULT_ERROR;
        }

        return I2CDEV_RESULT_OK;
    }

    auto writeReg16(std::uint8_t devAddr, std::uint8_t regAddr, std::size_t length, const std::uint16_t* data)
      -> i2cdev_result_t override
    {
        this->wire_->beginTransmission(devAddr);

        // Send address
        if (this->wire_->write(regAddr) != 1) {
            return I2CDEV_RESULT_ERROR;
        }

        // Send data
        for (std::size_t i = 0; i < length; i++) {
            // Send MSB
            if (this->wire_->write(static_cast<std::uint8_t>(data[i] >> 8)) != 1) {
                return I2CDEV_RESULT_ERROR;
            }

            // Send LSB
            if (this->wire_->write(static_cast<std::uint8_t>(data[i] & 0xFF)) != 1) {
                return I2CDEV_RESULT_ERROR;
            }
        }

        if (this->wire_->endTransmission() != 0) {
            return I2CDEV_RESULT_ERROR;
        }

        return I2CDEV_RESULT_OK;
    }
};

ArduinoI2CDev I2CDev0 = ArduinoI2CDev(&Wire);

#ifdef ARDUINO_ESP32_DEV
ArduinoI2CDev I2CDev1 = ArduinoI2CDev(&Wire1);
#endif // ESP32

#ifndef I2CDEV_DEFAULT_BUS
#define I2CDEV_DEFAULT_BUS I2CDev0
#endif // I2CDEV_DEFAULT_DEV

#endif // ARDUINO