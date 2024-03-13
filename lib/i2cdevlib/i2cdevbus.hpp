#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "i2cdevbus.h"

/// Inspired by the `jrowberg/i2cdevlib` project, but refactored to be more abstract and modern C++.
class I2CDevBus {
  public:
    static const uint16_t DEFAULT_READ_TIMEOUT_MS = I2CDEV_DEFAULT_READ_TIMEOUT_MS;

    /// Read multiple bytes from an 8-bit device register.
    ///
    /// \param [in] devAddr I2C slave device address
    /// \param [in] regAddr Register address to start reading from
    /// \param [in] length Number of bytes to read
    /// \param [out] data Buffer to store read data in
    /// \param [in] timeout Read operation timeout (default DEFAULT_READ_TIMEOUT). Set to 0 to disable timeout.
    ///
    /// \return 0 on success, negative error code on failure
    /// \retval I2CDEV_RESULT_OK If the read operation was successful
    [[nodiscard]] virtual auto readReg8(
      const i2cdev_dev_addr_t devAddr,
      const i2cdev_reg_addr_t regAddr,
      const std::size_t length,
      std::uint8_t* const data,
      const std::uint16_t timeout = DEFAULT_READ_TIMEOUT_MS
    ) -> i2cdev_result_t = 0;

    /// Read a single byte from an 8-bit device register.
    ///
    /// \param [in] devAddr I2C slave device address
    /// \param [in] regAddr Register address to read from
    /// \param [out] data Buffer to store read data in
    /// \param [in] timeout Read operation timeout (default DEFAULT_READ_TIMEOUT). Set to 0 to disable timeout.
    ///
    /// \return 0 on success, negative error code on failure
    /// \retval I2CDEV_RESULT_OK If the read operation was successful
    [[nodiscard]] inline auto readReg8(
      const i2cdev_dev_addr_t devAddr,
      const i2cdev_reg_addr_t regAddr,
      std::uint8_t* const data,
      const std::uint16_t timeout = DEFAULT_READ_TIMEOUT_MS
    ) -> i2cdev_result_t
    {
        return this->readReg8(devAddr, regAddr, 1, data, timeout);
    }

    /// Write multiple bytes to an 8-bit device register.
    ///
    /// \return 0 on success, negative error code on failure
    /// \retval I2CDEV_RESULT_OK If the read operation was successful
    [[nodiscard]] virtual auto writeReg8(
      const i2cdev_dev_addr_t devAddr,
      const i2cdev_reg_addr_t regAddr,
      const std::size_t length,
      const std::uint8_t* const data
    ) -> i2cdev_result_t = 0;

    /// Write multiple bytes to an 8-bit device register.
    ///
    /// \return 0 on success, negative error code on failure
    /// \retval I2CDEV_RESULT_OK If the read operation was successful
    template<typename U, typename = std::enable_if_t<std::is_same_v<typename U::value_type, std::uint8_t>>>
    [[nodiscard]] auto writeReg8(const i2cdev_dev_addr_t devAddr, const i2cdev_reg_addr_t regAddr, const U& data)
      -> i2cdev_result_t
    {
        return this->writeReg8(devAddr, regAddr, data.size(), data.data());
    }

    /// Write a single byte to an 8-bit device register.
    ///
    /// \return 0 on success, negative error code on failure
    /// \retval I2CDEV_RESULT_OK If the read operation was successful
    [[nodiscard]] inline auto
      writeReg8(const i2cdev_dev_addr_t devAddr, const i2cdev_reg_addr_t regAddr, const std::uint8_t data)
        -> i2cdev_result_t
    {
        return this->writeReg8(devAddr, regAddr, 1, &data);
    }

    /// Update a single byte on an 8-bit device register.
    ///
    /// \note If the calculated new register value matches the value that
    /// was read this function will not generate a write operation.
    ///
    /// \param [in] devAddr I2C slave device address
    /// \param [in] regAddr Register address to update
    /// \param [in] value New value to write to the register
    /// \param [in] mask Bitmask for updating internal register.
    ///
    /// \return 0 on success, negative error code on failure
    /// \retval I2CDEV_RESULT_OK If the read operation was successful
    [[nodiscard]] auto updateReg8(
      const i2cdev_dev_addr_t devAddr,
      const i2cdev_reg_addr_t regAddr,
      const std::uint8_t mask,
      const std::uint8_t value
    ) -> i2cdev_result_t
    {
        std::uint8_t old_data;

        const auto read_result = this->readReg8(devAddr, regAddr, &old_data);
        if (read_result != I2CDEV_RESULT_OK) {
            return read_result;
        }

        const std::uint8_t new_data = (old_data & ~mask) | (value & mask);
        if (old_data == new_data) {
            return I2CDEV_RESULT_OK;
        }

        return this->writeReg8(devAddr, regAddr, new_data);
    }

    [[nodiscard]] inline auto updateReg8Bits(
      const i2cdev_dev_addr_t devAddr,
      const i2cdev_reg_addr_t regAddr,
      const std::uint8_t startBit,
      const std::uint8_t length,
      const std::uint8_t value
    ) -> i2cdev_result_t
    {
        const std::uint8_t mask = (1 << length) - 1;
        return this->updateReg8(devAddr, regAddr, mask << startBit, value << startBit);
    }

    [[nodiscard]] inline auto updateReg8Bit(
      const i2cdev_dev_addr_t devAddr, const i2cdev_reg_addr_t regAddr, const std::uint8_t bit, const bool value
    ) -> i2cdev_result_t
    {
        return this->updateReg8(devAddr, regAddr, value ? (1 << bit) : 0, 1 << bit);
    }

    /// Read multiple words from a 16-bit device register.
    ///
    /// \param [in] devAddr I2C slave device address
    /// \param [in] regAddr Register address to start reading from
    /// \param [in] length Number of words to read
    /// \param [out] data Buffer to store read data in
    /// \param [in] timeout Read operation timeout (default DEFAULT_READ_TIMEOUT). Set to 0 to disable timeout.
    ///
    /// \return 0 on success, negative error code on failure
    /// \retval I2CDEV_RESULT_OK If the read operation was successful
    [[nodiscard]] virtual auto readReg16(
      const i2cdev_dev_addr_t devAddr,
      const i2cdev_reg_addr_t regAddr,
      const std::size_t length,
      std::uint16_t* const data,
      const std::uint16_t timeout = DEFAULT_READ_TIMEOUT_MS
    ) -> i2cdev_result_t = 0;

    /// Read a single word from a 16-bit device register.
    ///
    /// \param [in] devAddr I2C slave device address
    /// \param [in] regAddr Register address to read from
    /// \param [out] data Buffer to store read data in
    /// \param [in] timeout Read operation timeout (default DEFAULT_READ_TIMEOUT). Set to 0 to disable timeout.
    ///
    /// \return 0 on success, negative error code on failure
    /// \retval I2CDEV_RESULT_OK If the read operation was successful
    [[nodiscard]] inline auto readReg16(
      const i2cdev_dev_addr_t devAddr,
      const i2cdev_reg_addr_t regAddr,
      std::uint16_t* const data,
      const std::uint16_t timeout = DEFAULT_READ_TIMEOUT_MS
    ) -> i2cdev_result_t
    {
        return this->readReg16(devAddr, regAddr, 1, data, timeout);
    }

    /// Write multiple words to a 16-bit device register.
    ///
    /// \param [in] devAddr I2C slave device address
    /// \param [in] regAddr Register address to start writing to
    /// \param [in] length Number of words to write
    /// \param [in] data Buffer to write data from
    ///
    /// \return 0 on success, negative error code on failure
    /// \retval I2CDEV_RESULT_OK If the read operation was successful
    [[nodiscard]] virtual auto writeReg16(
      const i2cdev_dev_addr_t devAddr,
      const i2cdev_reg_addr_t regAddr,
      const std::size_t length,
      const std::uint16_t* data
    ) -> i2cdev_result_t = 0;

    /// Write multiple words to a 16-bit device register.
    ///
    /// \return 0 on success, negative error code on failure
    /// \retval I2CDEV_RESULT_OK If the read operation was successful
    template<typename U, typename = std::enable_if_t<std::is_same_v<typename U::value_type, std::uint16_t>>>
    [[nodiscard]] auto writeReg16(const i2cdev_dev_addr_t devAddr, const i2cdev_reg_addr_t regAddr, const U& data)
      -> i2cdev_result_t
    {
        return this->writeReg16(devAddr, regAddr, data.size(), data.data());
    }

    /// Write a single word to a 16-bit device register.
    ///
    /// \return 0 on success, negative error code on failure
    /// \retval I2CDEV_RESULT_OK If the read operation was successful
    [[nodiscard]] inline auto
      writeReg16(const i2cdev_dev_addr_t devAddr, const i2cdev_reg_addr_t regAddr, const std::uint16_t data)
        -> i2cdev_result_t
    {
        return this->writeReg16(devAddr, regAddr, 1, &data);
    }

    /// Update a single word on a 16-bit device register.
    ///
    /// \note If the calculated new register value matches the value that
    /// was read this function will not generate a write operation.
    ///
    /// \param [in] devAddr I2C slave device address
    /// \param [in] regAddr Register address to update
    /// \param [in] value New value to write to the register
    /// \param [in] mask Bitmask for updating internal register.
    ///
    /// \return 0 on success, negative error code on failure
    /// \retval I2CDEV_RESULT_OK If the read operation was successful
    [[nodiscard]] auto updateReg16(
      const i2cdev_dev_addr_t devAddr,
      const i2cdev_reg_addr_t regAddr,
      const std::uint16_t mask,
      const std::uint16_t value
    ) -> i2cdev_result_t
    {
        std::uint16_t old_data;

        const auto read_result = this->readReg16(devAddr, regAddr, &old_data);
        if (read_result != I2CDEV_RESULT_OK) {
            return read_result;
        }

        const std::uint16_t new_data = (old_data & ~mask) | (value & mask);
        if (old_data == new_data) {
            return I2CDEV_RESULT_OK;
        }

        return this->writeReg16(devAddr, regAddr, new_data);
    }

    [[nodiscard]] inline auto updateReg16Bits(
      const i2cdev_dev_addr_t devAddr,
      const i2cdev_reg_addr_t regAddr,
      const std::uint8_t startBit,
      const std::uint8_t length,
      const std::uint16_t value
    ) -> i2cdev_result_t
    {
        const std::uint16_t mask = (1U << length) - 1;
        return this->updateReg16(devAddr, regAddr, mask << startBit, value << startBit);
    }

    [[nodiscard]] inline auto updateReg16Bit(
      const i2cdev_dev_addr_t devAddr, const i2cdev_reg_addr_t regAddr, const std::uint8_t bit, const bool value
    ) -> i2cdev_result_t
    {
        return this->updateReg16(devAddr, regAddr, value ? (1 << bit) : 0, 1 << bit);
    }
};
