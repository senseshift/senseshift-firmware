#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <Arduino.h>

#include "senseshift/arduino/input/sensor/analog.hpp"

#include <senseshift/input/sensor.hpp>

namespace SenseShift::Arduino::Input {
    class CD74HC4057Component : public IInitializable {
      public:
        /// \param pin_s0 The S0 pin.
        /// \param pin_s1 The S1 pin.
        /// \param pin_s2 The S2 pin.
        /// \param pin_s3 The S3 pin.
        /// \param switch_delay_us The delay in microseconds to wait after switching the channel.
        CD74HC4057Component(
          const std::uint8_t pin_s0,
          const std::uint8_t pin_s1,
          const std::uint8_t pin_s2,
          const std::uint8_t pin_s3,
          const size_t switch_delay_us = 2
        ) :
          pin_s0_(pin_s0), pin_s1_(pin_s1), pin_s2_(pin_s2), pin_s3_(pin_s3), switch_delay_us_(switch_delay_us)
        {
        }

        void init() override
        {
            pinMode(this->pin_s0_, OUTPUT);
            pinMode(this->pin_s1_, OUTPUT);
            pinMode(this->pin_s2_, OUTPUT);
            pinMode(this->pin_s3_, OUTPUT);
        }

        /// Select the channel.
        /// \param channel The channel to select.
        ///
        /// \note
        /// Here is a table of the channel to select:
        /// | Channel | S0 | S1 | S2 | S3 |
        /// |---------|----|----|----|----|
        /// | 0       | 0  | 0  | 0  | 0  |
        /// | 1       | 1  | 0  | 0  | 0  |
        /// | 2       | 0  | 1  | 0  | 0  |
        /// | 3       | 1  | 1  | 0  | 0  |
        /// | 4       | 0  | 0  | 1  | 0  |
        /// | 5       | 1  | 0  | 1  | 0  |
        /// | 6       | 0  | 1  | 1  | 0  |
        /// | 7       | 1  | 1  | 1  | 0  |
        /// | 8       | 0  | 0  | 0  | 1  |
        /// | 9       | 1  | 0  | 0  | 1  |
        /// | 10      | 0  | 1  | 0  | 1  |
        /// | 11      | 1  | 1  | 0  | 1  |
        /// | 12      | 0  | 0  | 1  | 1  |
        /// | 13      | 1  | 0  | 1  | 1  |
        /// | 14      | 0  | 1  | 1  | 1  |
        /// | 15      | 1  | 1  | 1  | 1  |
        void selectChannel(const std::uint8_t channel)
        {
            if (this->active_channel_ == channel) {
                return;
            }

            digitalWrite(this->pin_s0_, channel & 0b00001);
            digitalWrite(this->pin_s1_, channel & 0b00010);
            digitalWrite(this->pin_s2_, channel & 0b00100);
            digitalWrite(this->pin_s3_, channel & 0b01000);

            delayMicroseconds(this->switch_delay_us_);

            this->active_channel_ = channel;
        }

      private:
        const std::uint8_t pin_s0_;
        const std::uint8_t pin_s1_;
        const std::uint8_t pin_s2_;
        const std::uint8_t pin_s3_;

        size_t active_channel_ = 0;

        const size_t switch_delay_us_;
    };

    template<bool Invert = false>
    class CD74HC4057Sensor : public AnalogSimpleSensor<Invert> {
        CD74HC4057Component* component_;
        const std::uint8_t channel_;

      public:
        /// \param component The CD74HC4057 Component to use.
        /// \param pin The SIG pin of the sensor.
        /// \param channel The channel to read from.
        CD74HC4057Sensor(CD74HC4057Component* component, const std::uint8_t pin, const std::uint8_t channel) :
          component_(component), AnalogSimpleSensor<Invert>(pin), channel_(channel)
        {
            assert(channel < 16 && "Channel must be between 0 and 15");
        }

        void init() override
        {
            this->component_->init();
            AnalogSimpleSensor<Invert>::init();
        }

        [[nodiscard]] auto getValue() -> float override
        {
            this->component_->selectChannel(this->channel_);

            return AnalogSimpleSensor<Invert>::getValue();
        }
    };
} // namespace SenseShift::Arduino::Input
