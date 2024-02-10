#pragma once

#include <Arduino.h>

#include <array>
#include <cstddef>
#include <optional>

#include "senseshift/arduino/input/sensor/analog.hpp"
#include "senseshift/arduino/input/sensor/digital.hpp"
#include "senseshift/core/component.hpp"
#include "senseshift/freertos/task.hpp"
#include "senseshift/input/sensor.hpp"
#include "senseshift/opengloves/opengloves.hpp"
#include "senseshift/utility.hpp"
#include <opengloves/opengloves.hpp>
#include <utility>

#define SS_START_CALIBRATION_NOT_NULL(ptr) \
    if ((ptr) != nullptr) {                \
        (ptr)->startCalibration();         \
    }

namespace SenseShift::OpenGloves {
    class OpenGlovesTrackingComponent : public SenseShift::Component {
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
            this->encoder_ = new og::AlphaEncoder();
        }

        void init() override
        {
            this->communication_->init();
            this->input_sensors_.init();

            if (this->config_.always_calibrate_) {
                this->startCalibration();
            }
        }

        void tick() override
        {
            this->input_sensors_.tick();
            const auto data = this->input_sensors_.collectData();
            const auto length = this->encoder_->encode_input(data, buffer.data(), buffer.size());
            this->communication_->send(buffer.data(), length);
        }

      protected:
        void startCalibration()
        {
            for (auto& finger_curl : this->input_sensors_.curl.fingers) {
                for (auto& joint_sensor : finger_curl.curl) {
                    SS_START_CALIBRATION_NOT_NULL(joint_sensor);
                }
            }

            for (auto& finger_splay : this->input_sensors_.splay.fingers) {
                SS_START_CALIBRATION_NOT_NULL(finger_splay);
            }

            SS_START_CALIBRATION_NOT_NULL(this->input_sensors_.joystick.x);
            SS_START_CALIBRATION_NOT_NULL(this->input_sensors_.joystick.y);
            SS_START_CALIBRATION_NOT_NULL(this->input_sensors_.joystick.press);

            for (auto& button : this->input_sensors_.buttons) {
                SS_START_CALIBRATION_NOT_NULL(button.press);
            }

            for (auto& analog_button : this->input_sensors_.analog_buttons) {
                SS_START_CALIBRATION_NOT_NULL(analog_button.press);
                SS_START_CALIBRATION_NOT_NULL(analog_button.value);
            }
        }

      private:
        std::array<char, 256> buffer;

        Config& config_;
        InputSensors input_sensors_;
        ITransport* communication_;
        og::IEncoder* encoder_;
    };

    //    class OpenGlovesForceFeedbackTask : public SenseShift::FreeRTOS::Task<OpenGlovesForceFeedbackTask> {
    //        friend class SenseShift::FreeRTOS::Task<OpenGlovesForceFeedbackTask>;
    //
    //      public:
    //        OpenGlovesForceFeedbackTask(
    //          ::SenseShift::OpenGloves::ITransport& communication,
    //          HandActuators& actuators,
    //          size_t updateRate,
    //          SenseShift::FreeRTOS::TaskConfig taskConfig
    //        ) :
    //          communication(communication),
    //          actuators(actuators),
    //          SenseShift::FreeRTOS::Task<OpenGlovesForceFeedbackTask>(taskConfig)
    //        {
    //            this->updateIntervalMs = 1000 / updateRate;
    //        };
    //
    //        void begin() override
    //        {
    //            log_d("Starting OpenGloves force feedback task: %p", this);
    //            this->setup();
    //            this->SenseShift::FreeRTOS::Task<OpenGlovesForceFeedbackTask>::begin();
    //        };
    //
    //      private:
    //        ::SenseShift::OpenGloves::ITransport& communication;
    //        HandActuators& actuators;
    //        size_t updateIntervalMs;
    //        ::SenseShift::OpenGloves::AlphaEncodingService encodingService =
    //          ::SenseShift::OpenGloves::AlphaEncodingService();
    //
    //        void setup()
    //        {
    //            log_d("Setting up OpenGloves force feedback task: %p", this);
    //            this->communication.setup();
    //
    //            if (this->actuators.thumb.has_value()) {
    //                this->actuators.thumb.value()->setup();
    //            }
    //
    //            if (this->actuators.index.has_value()) {
    //                this->actuators.index.value()->setup();
    //            }
    //
    //            if (this->actuators.middle.has_value()) {
    //                this->actuators.middle.value()->setup();
    //            }
    //
    //            if (this->actuators.ring.has_value()) {
    //                this->actuators.ring.value()->setup();
    //            }
    //
    //            if (this->actuators.pinky.has_value()) {
    //                this->actuators.pinky.value()->setup();
    //            }
    //        }
    //
    //        void run()
    //        {
    //            char commandBuffer[256];
    //            std::string command;
    //            while (true) {
    //                auto now = millis();
    //
    //                if (this->communication.hasData()) {
    //                    auto bytesRead = this->communication.read(commandBuffer, sizeof(commandBuffer));
    //                    if (bytesRead == 0) {
    //                        continue;
    //                    }
    //
    //                    std::map<::OpenGloves::Command, uint16_t> commands = {};
    //                    this->encodingService.deserialize(commandBuffer, bytesRead, commands);
    //
    //                    for (auto& [command, value] : commands) {
    //                        this->handleCommand(command, value);
    //                    }
    //                }
    //
    //                // Delay until the next update.
    //                auto elapsed = millis() - now;
    //                if (elapsed < this->updateIntervalMs) {
    //                    delay(this->updateIntervalMs - elapsed);
    //                }
    //            }
    //        }
    //
    //        void handleCommand(Command command, uint16_t value)
    //        {
    //            switch (command) {
    //                case Command::ThumbCurl:
    //                    if (this->actuators.thumb.has_value()) {
    //                        this->actuators.thumb.value()->writeOutput(value);
    //                    }
    //                    break;
    //                case Command::IndexCurl:
    //                    if (this->actuators.index.has_value()) {
    //                        this->actuators.index.value()->writeOutput(value);
    //                    }
    //                    break;
    //                case Command::MiddleCurl:
    //                    if (this->actuators.middle.has_value()) {
    //                        this->actuators.middle.value()->writeOutput(value);
    //                    }
    //                    break;
    //                case Command::RingCurl:
    //                    if (this->actuators.ring.has_value()) {
    //                        this->actuators.ring.value()->writeOutput(value);
    //                    }
    //                    break;
    //                case Command::PinkyCurl:
    //                    if (this->actuators.pinky.has_value()) {
    //                        this->actuators.pinky.value()->writeOutput(value);
    //                    }
    //                    break;
    //                default:
    //                    log_w("Unhandled command: %d", command);
    //                    break;
    //            }
    //        }
    //    };
} // namespace SenseShift::OpenGloves
