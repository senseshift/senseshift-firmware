#pragma once

#include <cstddef>
#include <cstdint>

#include "i2cdev.hpp"

#ifdef ARDUINO

#include <Wire.h>

class ArduinoI2CDev : public I2CDev {
  public:
    explicit ArduinoI2CDev(TwoWire* wire = &Wire) : wire_(wire) {}

  private:
    TwoWire* wire_;

    auto readBytes(
      std::uint8_t devAddr,
      std::uint8_t regAddr,
      std::size_t length,
      std::uint8_t* data,
      std::uint16_t timeout = DEFAULT_READ_TIMEOUT
    ) -> std::int8_t override
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
                return received;
            }
        }

        if (timeout > 0 && millis() - start >= timeout && received < length) {
            return -1;
        }

        return received;
    }

    auto readWords(
      std::uint8_t devAddr,
      std::uint8_t regAddr,
      std::size_t length,
      std::uint16_t* data,
      std::uint16_t timeout = DEFAULT_READ_TIMEOUT
    ) -> std::int8_t override
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
            if (received == length) {
                return received;
            }
        }

        if (timeout > 0 && millis() - start >= timeout && received < length * 2) {
            return -1;
        }

        return received / 2;
    }

    auto writeBytes(std::uint8_t devAddr, std::uint8_t regAddr, std::size_t length, const std::uint8_t* data)
      -> bool override
    {
        this->wire_->beginTransmission(devAddr);

        // Send address
        if (this->wire_->write(regAddr) != 1) {
            return false;
        }

        // Send data
        if (this->wire_->write(data, length) != length) {
            return false;
        }

        return this->wire_->endTransmission() == 0;
    }

    auto writeWords(std::uint8_t devAddr, std::uint8_t regAddr, std::size_t length, const std::uint16_t* data)
      -> bool override
    {
        this->wire_->beginTransmission(devAddr);

        // Send address
        if (this->wire_->write(regAddr) != 1) {
            return false;
        }

        // Send data
        for (std::size_t i = 0; i < length; i++) {
            // Send MSB
            if (this->wire_->write(static_cast<std::uint8_t>(data[i] >> 8)) != 1) {
                return false;
            }

            // Send LSB
            if (this->wire_->write(static_cast<std::uint8_t>(data[i] & 0xFF)) != 1) {
                return false;
            }
        }

        return this->wire_->endTransmission() == 0;
    }
};

ArduinoI2CDev I2CDev0 = ArduinoI2CDev(&Wire);

#ifdef ARDUINO_ESP32_DEV
ArduinoI2CDev I2CDev1 = ArduinoI2CDev(&Wire1);
#endif // ESP32

#ifndef I2CDEV_DEFAULT_DEV
#define I2CDEV_DEFAULT_DEV I2CDev0
#endif // I2CDEV_DEFAULT_DEV

#endif // ARDUINO