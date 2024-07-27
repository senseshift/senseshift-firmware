#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>

#include <Arduino.h>

#include "senseshift/arduino/input/sensor/analog.hpp"
#include "senseshift/arduino/input/sensor/digital.hpp"

#include <senseshift/core/component.hpp>
#include <senseshift/input/sensor/sensor.hpp>

namespace SenseShift::Arduino::Input {
template<size_t N>
class Multiplexer : public IInitializable {
  public:
    /// \param pins The pins to use for the multiplexer.
    explicit Multiplexer(const std::array<std::uint8_t, N>& pins, const size_t switch_delay_us = 2) :
      pins_(pins), switch_delay_us_(switch_delay_us)
    {
    }

    void init() override
    {
        for (const auto pin : this->pins_) {
            pinMode(pin, OUTPUT);
        }
    }

    /// Select the channel.
    /// \param channel The channel to select.
    void selectChannel(const std::uint8_t channel)
    {
        if (channel >= (1 << N) || this->active_channel_ == channel) {
            return;
        }

        for (size_t i = 0; i < N; ++i) {
            digitalWrite(this->pins_[i], (channel >> i) & 0b0001);
        }

        delayMicroseconds(this->switch_delay_us_);

        this->active_channel_ = channel;
    }

  private:
    const std::array<std::uint8_t, N> pins_;
    std::uint8_t active_channel_ = 0;
    const size_t switch_delay_us_;
};

using MUX_CD74HC4057Component = Multiplexer<4>;
using MUX_74HC4051Component = Multiplexer<3>;

template<size_t N>
class MultiplexedAnalogSensor : public AnalogSimpleSensor {
  public:
    /// \param component The CD74HC4057 Component to use.
    /// \param pin The SIG pin of the sensor.
    /// \param channel The channel to read from.
    MultiplexedAnalogSensor(Multiplexer<N>* component, const std::uint8_t pin_sig, const std::uint8_t channel) :
      component_(component), AnalogSimpleSensor(pin_sig), channel_(channel)
    {
        assert(channel < (1 << N) && "Channel out of range");
    }

    void init() override
    {
        this->component_->init();
        AnalogSimpleSensor::init();
    }

    [[nodiscard]] auto getValue() -> float override
    {
        this->component_->selectChannel(this->channel_);

        return AnalogSimpleSensor::getValue();
    }

  private:
    Multiplexer<N>* component_;
    const std::uint8_t channel_;
};

class MultiplexedDigitalSensor : public DigitalSimpleSensor {
  public:
    /// \param component The CD74HC4057 Component to use.
    /// \param pin The SIG pin of the sensor.
    /// \param channel The channel to read from.
    MultiplexedDigitalSensor(
      MUX_CD74HC4057Component* component, const std::uint8_t pin_sig, const std::uint8_t channel
    ) :
      component_(component), DigitalSimpleSensor(pin_sig), channel_(channel)
    {
        assert(channel < 16 && "Channel out of range");
    }

    void init() override
    {
        this->component_->init();
        DigitalSimpleSensor::init();
    }

    [[nodiscard]] auto getValue() -> bool override
    {
        this->component_->selectChannel(this->channel_);

        return DigitalSimpleSensor::getValue();
    }

  private:
    MUX_CD74HC4057Component* component_;
    const std::uint8_t channel_;
};

} // namespace SenseShift::Arduino::Input
