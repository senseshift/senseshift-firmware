#pragma once

#include <cstdint>

#include <esp32-hal-ledc.h>

#include <senseshift/output/output.hpp>
#include <senseshift/core/logging.hpp>

namespace SenseShift::Arduino::Output {
    static const char *const TAG = "output.ledc";

    /// Arduino analog output
    class LedcOutput : public ::SenseShift::Output::IFloatOutput {
    public:
        explicit LedcOutput(
          const std::uint8_t pin,
          const std::uint8_t analog_resolution = 12,
          const std::uint32_t analog_frequency = 60
        ) : pin_(pin), analog_resolution_(analog_resolution), analog_frequency_(analog_frequency) {}

        void init() override {
            pinMode(this->pin_, OUTPUT);
            this->channel_ = findChannel(this->pin_);

            LOG_D(TAG, "GPIO %d - Setting up Channel %d", this->pin_, this->channel_);
            if(ledcSetup(this->channel_, this->analog_frequency_, this->analog_resolution_) == 0){
                LOG_E(TAG, "setup failed (freq = %u, resolution = %u). Try setting different resolution or frequency", this->analog_frequency_, this->analog_resolution_);
                return;
            }
            ledcAttachPin(this->pin_, this->channel_);
        }

        [[nodiscard]] auto getMaxValue() const -> std::uint32_t {
            return (1 << analog_resolution_) - 1;
        }

        void writeState(const float value) override {
            const auto duty = static_cast<std::uint32_t>(value * this->getMaxValue());
            LOG_V(TAG, "GPIO %d - Writing %d to Channel %d", this->pin_, duty, this->channel_);
            ledcWrite(this->channel_, duty);
        };

    protected:
        static auto findChannel(std::uint8_t /*pin*/) -> std::int8_t {
            return CHANNELS++;
        }

    private:
        static inline std::uint8_t CHANNELS = 0;

        std::uint8_t pin_;
        std::int8_t channel_ = -1;
        std::uint8_t analog_resolution_;
        std::uint32_t analog_frequency_;
    };
} // namespace SenseShift::Arduino::Output
