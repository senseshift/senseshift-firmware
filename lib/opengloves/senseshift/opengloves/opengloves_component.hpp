#pragma once

#include <Arduino.h>

#include <array>
#include <cstddef>
#include <optional>
#include <utility>

#include <opengloves.hpp>

#include "senseshift/opengloves/opengloves.hpp"
#include <senseshift/opengloves/opengloves_plotter.hpp>

#include <senseshift/core/component.hpp>

namespace SenseShift::OpenGloves {
template<typename T>
class OpenGlovesTrackingComponent : public SenseShift::Component {
    // Plotter raw_plotter_ = Plotter(&Serial, "Raw");
    // Plotter calibrated_plotter_ = Plotter(&Serial, "Cal");

  public:
    class Config {
        friend class OpenGlovesTrackingComponent;
        size_t calibration_duration_ms_;
        bool always_calibrate_;

      public:
        /// \param updateRate The rate at which the sensors should be updated in Hz.
        /// \param calibrationDuration The duration in milliseconds that the calibration button should be held for.
        Config(size_t calibration_duration_ms, bool always_calibrate) :
          calibration_duration_ms_(calibration_duration_ms), always_calibrate_(always_calibrate)
        {
        }
    };

    OpenGlovesTrackingComponent(Config& config, InputSensors& input_sensors, ITransport* communication) :
      config_(config), input_sensors_(std::move(input_sensors)), communication_(communication)
    {
    }

    void init() override
    {
        this->communication_->init();
        this->input_sensors_.init();

        // If the calibration button is not present, start calibration immediately.
        if (this->config_.always_calibrate_ || this->input_sensors_.button_calibrate.press == nullptr) {
            this->startCalibration();
        }
    }

    void tick() override
    {
        // const auto start = micros();

        // auto now = micros();
        this->input_sensors_.tick();
        // const auto tickTime = micros() - now;

        // now = micros();
        const auto data = this->input_sensors_.collectData();
        // const auto collectTime = micros() - now;

        // const auto raw_data = this->input_sensors_.collectRawData();
        // this->raw_plotter_.plot(raw_data);
        // this->calibrated_plotter_.plot(data);

        bool const calibrate_pressed = data.button_calibrate.press;
        if (calibrate_pressed && this->calibration_start_time_ == 0) {
            this->startCalibration();
        }

        // now = micros();
        const auto length = T::encodeInput(data, reinterpret_cast<uint8_t*>(buffer.data()), buffer.size());
        // const auto encodeTime = micros() - now;

        // now = micros();
        this->communication_->send(buffer.data(), length);
        // const auto sendTime = micros() - now;

        if (!this->config_.always_calibrate_ && this->calibration_start_time_ != 0) {
            const auto calibration_elapsed = millis() - this->calibration_start_time_;
            const bool calibration_done = calibration_elapsed >= this->config_.calibration_duration_ms_;

            if (calibration_done) {
                this->stopCalibration();
            }
        }

        // const auto total = micros() - start;
        // log_i(
        //   "total: %d, tick: %d, collect: %d, encode: %d, send: %d, c/s: %.2f",
        //   total,
        //   tickTime,
        //   collectTime,
        //   encodeTime,
        //   sendTime,
        //   1000000.0 / total
        // );
    }

  protected:
    void startCalibration()
    {
        log_i("Starting calibration");
        this->input_sensors_.reselCalibration();
        this->input_sensors_.startCalibration();
        this->calibration_start_time_ = millis();
    }

    void stopCalibration()
    {
        log_i("Stopping calibration");
        this->input_sensors_.stopCalibration();
        this->calibration_start_time_ = 0;
    }

  private:
    std::array<char, 256> buffer = {};

    unsigned long long calibration_start_time_ = 0;

    Config& config_;
    InputSensors input_sensors_;
    ITransport* communication_;
};

template<typename T>
class OpenGlovesForceFeedbackComponent : public SenseShift::Component {
  public:
    OpenGlovesForceFeedbackComponent(
      OutputWriters& output_writers, ::SenseShift::OpenGloves::ITransport* communication
    ) :
      output_writers_(output_writers), communication_(communication)
    {
    }

    void init() override
    {
        log_d("Setting up OpenGloves force feedback task: %p", this);
        this->communication_->init();
        this->output_writers_.init();
    }

    void tick() override
    {
        if (this->communication_->hasData()) {
            const auto length = this->communication_->read(this->buffer.data(), this->buffer.size());
            const auto output = T::decodeOutput(reinterpret_cast<const uint8_t*>(this->buffer.data()), length);
            this->output_writers_.apply(output);
        }
    }

  private:
    std::array<char, 256> buffer = {};

    OutputWriters output_writers_;
    ::SenseShift::OpenGloves::ITransport* communication_;
};
} // namespace SenseShift::OpenGloves
