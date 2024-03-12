#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

/// 1000ms default read timeout (modify with `I2CDev::DEFAULT_READ_TIMEOUT = [ms];`)
#define I2CDEV_DEFAULT_READ_TIMEOUT 1000

/// Inspired by the `jrowberg/i2cdevlib` project, but refactored to be more abstract and modern C++.
class I2CDev {
  public:
    static const uint16_t DEFAULT_READ_TIMEOUT = I2CDEV_DEFAULT_READ_TIMEOUT;

    /// Read multiple bytes from an 8-bit device register.
    ///
    /// \param [in] devAddr I2C slave device address
    /// \param [in] regAddr Register address to start reading from
    /// \param [in] length Number of bytes to read
    /// \param [out] data Buffer to store read data in
    /// \param [in] timeout Read operation timeout (default DEFAULT_READ_TIMEOUT). Set to 0 to disable timeout.
    ///
    /// \return Number of bytes read (-1 indicates failure)
    [[nodiscard]] virtual auto readBytes(
      std::uint8_t devAddr,
      std::uint8_t regAddr,
      std::size_t length,
      std::uint8_t* data,
      std::uint16_t timeout = DEFAULT_READ_TIMEOUT
    ) -> std::int8_t = 0;

    /// Read a single byte from an 8-bit device register.
    ///
    /// \param [in] devAddr I2C slave device address
    /// \param [in] regAddr Register address to read from
    /// \param [out] data Buffer to store read data in
    /// \param [in] timeout Read operation timeout (default DEFAULT_READ_TIMEOUT). Set to 0 to disable timeout.
    ///
    /// \return True if successful, false otherwise
    [[nodiscard]] inline auto readByte(
      std::uint8_t devAddr, std::uint8_t regAddr, std::uint8_t* data, std::uint16_t timeout = DEFAULT_READ_TIMEOUT
    ) -> bool
    {
        return readBytes(devAddr, regAddr, 1, data, timeout) == 1;
    }

    /// Read multiple words from a 16-bit device register.
    ///
    /// \param [in] devAddr I2C slave device address
    /// \param [in] regAddr Register address to start reading from
    /// \param [in] length Number of words to read
    /// \param [out] data Buffer to store read data in
    /// \param [in] timeout Read operation timeout (default DEFAULT_READ_TIMEOUT). Set to 0 to disable timeout.
    ///
    /// \return Number of words read (-1 indicates failure)
    [[nodiscard]] virtual auto readWords(
      std::uint8_t devAddr,
      std::uint8_t regAddr,
      std::size_t length,
      std::uint16_t* data,
      std::uint16_t timeout = DEFAULT_READ_TIMEOUT
    ) -> std::int8_t = 0;

    /// Read a single word from a 16-bit device register.
    ///
    /// \param [in] devAddr I2C slave device address
    /// \param [in] regAddr Register address to read from
    /// \param [out] data Buffer to store read data in
    /// \param [in] timeout Read operation timeout (default DEFAULT_READ_TIMEOUT). Set to 0 to disable timeout.
    ///
    /// \return True if successful, false otherwise
    [[nodiscard]] inline auto readWord(
      std::uint8_t devAddr, std::uint8_t regAddr, std::uint16_t* data, std::uint16_t timeout = DEFAULT_READ_TIMEOUT
    ) -> bool
    {
        return readWords(devAddr, regAddr, 1, data, timeout) == 1;
    }

    /// Write multiple bytes to an 8-bit device register.
    [[nodiscard]] virtual auto
      writeBytes(std::uint8_t devAddr, std::uint8_t regAddr, std::size_t length, const std::uint8_t* data) -> bool = 0;

    /// Write multiple bytes to an 8-bit device register.
    template<typename U, typename = std::enable_if_t<std::is_same_v<typename U::value_type, std::uint8_t>>>
    [[nodiscard]] auto writeBytes(std::uint8_t devAddr, std::uint8_t regAddr, const U& data) -> bool
    {
        return writeBytes(devAddr, regAddr, data.size(), data.data());
    }

    /// Write a single byte to an 8-bit device register.
    [[nodiscard]] inline auto writeByte(std::uint8_t devAddr, std::uint8_t regAddr, std::uint8_t data) -> bool
    {
        return writeBytes(devAddr, regAddr, 1, &data);
    }

    /// Write multiple words to a 16-bit device register.
    ///
    /// \param [in] devAddr I2C slave device address
    /// \param [in] regAddr Register address to start writing to
    /// \param [in] length Number of words to write
    /// \param [in] data Buffer to write data from
    ///
    /// \return True if successful, false otherwise
    [[nodiscard]] virtual auto
      writeWords(std::uint8_t devAddr, std::uint8_t regAddr, std::size_t length, const std::uint16_t* data) -> bool = 0;

    /// Write multiple words to a 16-bit device register.
    template<typename U, typename = std::enable_if_t<std::is_same_v<typename U::value_type, std::uint16_t>>>
    [[nodiscard]] auto writeWords(std::uint8_t devAddr, std::uint8_t regAddr, const U& data) -> bool
    {
        return writeWords(devAddr, regAddr, data.size(), data.data());
    }

    /// Write a single word to a 16-bit device register.
    [[nodiscard]] inline auto writeWord(std::uint8_t devAddr, std::uint8_t regAddr, std::uint16_t data) -> bool
    {
        return writeWords(devAddr, regAddr, 1, &data);
    }
};
