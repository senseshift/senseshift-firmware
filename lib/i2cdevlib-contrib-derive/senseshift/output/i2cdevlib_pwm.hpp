#pragma once

#include <cstdint>

#include <senseshift/output/output.hpp>
#include <senseshift/utility.hpp>

#include <I2CDevLib.h>
#include <i2cdev/pca9685.hpp>

namespace SenseShift::Output {

template<typename T, std::uint16_t MAX_INTENSITY>
class I2CDevLibContribPWMOutput : public ::SenseShift::Output::IFloatOutput {
  public:
    I2CDevLibContribPWMOutput(T& driver, const std::uint8_t num) : driver_(driver), channel_(num){};

    void init() override
    {
        //
    }

    void writeState(const ValueType value) override
    {
        const auto duty = static_cast<std::uint16_t>(value * MAX_INTENSITY);
        this->driver_.setChannel(this->channel_, duty);
    }

  private:
    T& driver_;
    std::uint8_t channel_;
};

using PCA9685Output = I2CDevLibContribPWMOutput<i2cdev::PCA9685, PCA9685_VALUE_MAX>;
} // namespace SenseShift::Output
