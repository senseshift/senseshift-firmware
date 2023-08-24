#pragma once

#include <Arduino.h>

#include <optional>

#include <og_alpha_encoding.hpp>
#include <og_ffb.hpp>
#include <og_serial_communication.hpp>
#include <senseshift/arduino/input/sensor/analog.hpp>
#include <senseshift/arduino/input/sensor/digital.hpp>
#include <senseshift/calibration.hpp>
#include <senseshift/freertos/task.hpp>
#include <senseshift/input/sensor.hpp>
#include <senseshift/input/sensor/joystick.hpp>
#include <senseshift/utility.hpp>
#include <sensor/og_finger.hpp>
#include <sensor/og_gesture.hpp>

namespace OpenGloves {
    struct OpenGlovesTrackingTaskConfig {
        size_t updateIntervalMs;
        size_t calibrationDuration;
        bool alwaysCalibrate;

        /**
         * @param updateRate The rate at which the sensors should be updated in Hz.
         * @param calibrationDuration The duration in milliseconds that the
         * calibration button should be held for.
         */
        OpenGlovesTrackingTaskConfig(size_t updateRate, size_t calibrationDuration, bool alwaysCalibrate) :
          calibrationDuration(calibrationDuration), alwaysCalibrate(alwaysCalibrate)
        {
            // Convert the update rate to an interval in milliseconds.
            this->updateIntervalMs = 1000 / updateRate;
        }
    };

    class OpenGlovesTrackingTask : public SenseShift::FreeRTOS::Task<OpenGlovesTrackingTask> {
        friend class SenseShift::FreeRTOS::Task<OpenGlovesTrackingTask>;

      public:
        /**
         * @param config The configuration for the OpenGloves tracking task.
         * @param communication The communication interface.
         * @param fingers The finger sensors.
         * @param buttons The button sensors.
         * @param joysticks The joystick sensors.
         * @param otherSensors Other sensors that should be updated.
         * @param calibrationButton The calibration button sensor, optional.
         * @param taskConfig The task configuration.
         */
        OpenGlovesTrackingTask(
          OpenGlovesTrackingTaskConfig& config,
          ICommunication& communication,
          HandSensors& fingers,
          std::vector<StringEncodedMemoizedSensor<bool>*>& buttons,
          std::vector<StringEncodedMemoizedSensor<uint16_t>*>& joysticks,
          std::vector<IStringEncodedMemoizedSensor*>& otherSensors,
          std::optional<StringEncodedMemoizedSensor<bool>>& calibrationButton,
          SenseShift::FreeRTOS::TaskConfig taskConfig
        ) :
          config(config),
          communication(communication),
          fingers(fingers),
          buttons(buttons),
          joysticks(joysticks),
          otherSensors(otherSensors),
          calibrationButton(calibrationButton),
          allSensors(std::vector<IStringEncodedMemoizedSensor*>()),
          SenseShift::FreeRTOS::Task<OpenGlovesTrackingTask>(taskConfig)
        {
            if (fingers.thumb.has_value()) {
                auto* thumb = &fingers.thumb.value();
                this->calibrated.push_back(thumb);
                this->allSensors.push_back(thumb);
            }
            if (fingers.index.has_value()) {
                auto* index = &fingers.index.value();
                this->calibrated.push_back(index);
                this->allSensors.push_back(index);
            }
            if (fingers.middle.has_value()) {
                auto* middle = &fingers.middle.value();
                this->calibrated.push_back(middle);
                this->allSensors.push_back(middle);
            }
            if (fingers.ring.has_value()) {
                auto* ring = &fingers.ring.value();
                this->calibrated.push_back(ring);
                this->allSensors.push_back(ring);
            }
            if (fingers.pinky.has_value()) {
                auto* pinky = &fingers.pinky.value();
                this->calibrated.push_back(pinky);
                this->allSensors.push_back(pinky);
            }

            for (auto* button : buttons) {
                this->allSensors.push_back(button);
            }

            if (calibrationButton.has_value()) {
                this->allSensors.push_back(&calibrationButton.value());
            }

            for (auto* joystick : joysticks) {
                this->allSensors.push_back(joystick);
            }

            for (auto* otherSensor : otherSensors) {
                this->allSensors.push_back(otherSensor);
            }

            // sort all sensors by type for easier debugging
            std::sort(
              this->allSensors.begin(),
              this->allSensors.end(),
              [](IStringEncodedMemoizedSensor* a, IStringEncodedMemoizedSensor* b) {
                  return a->getType() < b->getType();
              }
            );
        }

        void begin() override
        {
            log_d("Starting OpenGloves tracking task: %p", this);
            this->setup();
            this->SenseShift::FreeRTOS::Task<OpenGlovesTrackingTask>::begin();
        };

      private:
        OpenGlovesTrackingTaskConfig& config;

        HandSensors& fingers;
        ICommunication& communication;
        std::vector<StringEncodedMemoizedSensor<bool>*>& buttons;
        std::vector<StringEncodedMemoizedSensor<uint16_t>*>& joysticks;
        std::vector<IStringEncodedMemoizedSensor*>& otherSensors;

        std::vector<IStringEncodedMemoizedSensor*> allSensors;

        std::optional<StringEncodedMemoizedSensor<bool>>& calibrationButton;
        std::vector<SenseShift::Calibration::ICalibrated*> calibrated =
          std::vector<SenseShift::Calibration::ICalibrated*>();
        unsigned long long calibrationStarted = 0;

        void startCalibration()
        {
            for (auto* input : this->calibrated) {
                input->resetCalibration();
                input->enableCalibration();
            }
            this->calibrationStarted = millis();
        }

        void setup()
        {
            log_d("Setting up OpenGloves tracking task: %p", this);
            log_d("There is a total of %d sensors", this->allSensors.size());
            for (auto* input : this->allSensors) {
                log_d("Setting up sensor: %c", input->getType());
                input->setup();
            }

            // Start calibration if no calibration button is present or if configured to always calibrate.
            if (!this->calibrationButton.has_value() || this->config.alwaysCalibrate) {
                log_d("Starting calibration on startup");
                this->startCalibration();
            }

            this->communication.setup();
        }

        void run()
        {
            while (true) {
                auto now = millis();

                // Update the sensors.
                for (auto* input : this->allSensors) {
                    input->updateValue();
                }

                // Update the calibration if calibration has not started, calibration is not configured to always run,
                // and the calibration button is present and pressed.
                if (calibrationStarted == 0 &&
                    !this->config.alwaysCalibrate &&
                    this->calibrationButton.has_value() &&
                    this->calibrationButton.value().getValue()
                ) {
                    log_d("Calibration started");
                    this->startCalibration();
                }

                // Send the sensor values.
                this->communication.send(this->allSensors);

                // Check if the calibration has finished.
                if (!(this->config.alwaysCalibrate) && calibrationStarted > 0 && (now - calibrationStarted) > CALIBRATION_DURATION) {
                    log_d("Calibration finished");
                    for (size_t i = 0; i < calibrated.size(); i++) {
                        auto* input = calibrated[i];
                        input->disableCalibration();
                    }
                    calibrationStarted = 0;
                }

                // Delay until the next update.
                auto elapsed = millis() - now;
                if (elapsed < this->config.updateIntervalMs) {
                    delay(this->config.updateIntervalMs - elapsed);
                }
            }
        };
    };

    class OpenGlovesForceFeedbackTask : public SenseShift::FreeRTOS::Task<OpenGlovesForceFeedbackTask> {
        friend class SenseShift::FreeRTOS::Task<OpenGlovesForceFeedbackTask>;

      public:
        OpenGlovesForceFeedbackTask(
          ICommunication& communication,
          HandActuators& actuators,
          size_t updateRate,
          SenseShift::FreeRTOS::TaskConfig taskConfig
        ) :
          communication(communication),
          actuators(actuators),
          SenseShift::FreeRTOS::Task<OpenGlovesForceFeedbackTask>(taskConfig)
        {
            this->updateIntervalMs = 1000 / updateRate;
        };

        void begin() override
        {
            log_d("Starting OpenGloves force feedback task: %p", this);
            this->setup();
            this->SenseShift::FreeRTOS::Task<OpenGlovesForceFeedbackTask>::begin();
        };

      private:
        ICommunication& communication;
        HandActuators& actuators;
        size_t updateIntervalMs;

        void setup()
        {
            log_d("Setting up OpenGloves force feedback task: %p", this);
            this->communication.setup();

            if (this->actuators.thumb.has_value()) {
                this->actuators.thumb.value()->setup();
            }

            if (this->actuators.index.has_value()) {
                this->actuators.index.value()->setup();
            }

            if (this->actuators.middle.has_value()) {
                this->actuators.middle.value()->setup();
            }

            if (this->actuators.ring.has_value()) {
                this->actuators.ring.value()->setup();
            }

            if (this->actuators.pinky.has_value()) {
                this->actuators.pinky.value()->setup();
            }
        }

        void run()
        {
            char commandBuffer[256];
            std::string command;
            while (true) {
                auto now = millis();

                if (this->communication.hasData()) {
                    auto bytesRead = this->communication.readCommand(commandBuffer, sizeof(commandBuffer));
                    if (bytesRead == 0) {
                        continue;
                    }

                    command.assign(commandBuffer, bytesRead);

                    auto commands = AlphaEncodingService::splitCommands(command);

                    for (auto& [command, value] : commands) {
                        this->handleCommand(command, value);
                    }
                }

                // Delay until the next update.
                auto elapsed = millis() - now;
                if (elapsed < this->updateIntervalMs) {
                    delay(this->updateIntervalMs - elapsed);
                }
            }
        }

        void handleCommand(Command command, uint16_t value)
        {
            switch (command) {
                case Command::ThumbCurl:
                    if (this->actuators.thumb.has_value()) {
                        this->actuators.thumb.value()->writeOutput(value);
                    }
                    break;
                case Command::IndexCurl:
                    if (this->actuators.index.has_value()) {
                        this->actuators.index.value()->writeOutput(value);
                    }
                    break;
                case Command::MiddleCurl:
                    if (this->actuators.middle.has_value()) {
                        this->actuators.middle.value()->writeOutput(value);
                    }
                    break;
                case Command::RingCurl:
                    if (this->actuators.ring.has_value()) {
                        this->actuators.ring.value()->writeOutput(value);
                    }
                    break;
                case Command::PinkyCurl:
                    if (this->actuators.pinky.has_value()) {
                        this->actuators.pinky.value()->writeOutput(value);
                    }
                    break;
                default:
                    log_w("Unhandled command: %d", command);
                    break;
            }
        }
    };
} // namespace OpenGloves
