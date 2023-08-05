#pragma once

#include <Arduino.h>

#include <optional>

#include <calibration.hpp>
#include <og_serial_communication.hpp>
#include <sensor.hpp>
#include <sensor/analog.hpp>
#include <sensor/digital.hpp>
#include <sensor/joystick.hpp>
#include <sensor/og_finger.hpp>
#include <sensor/og_gesture.hpp>
#include <task.hpp>
#include <utility.hpp>

namespace OpenGloves {
    struct OpenGlovesConfig {
        size_t updateInterval;
        size_t calibrationDuration;
        bool alwaysCalibrate;

        /**
         * @param updateRate The rate at which the sensors should be updated in Hz.
         * @param calibrationDuration The duration in milliseconds that the
         * calibration button should be held for.
         */
        OpenGlovesConfig(size_t updateRate, size_t calibrationDuration, bool alwaysCalibrate) :
          calibrationDuration(calibrationDuration), alwaysCalibrate(alwaysCalibrate)
        {
            // Convert the update rate to an interval in milliseconds.
            this->updateInterval = 1000 / updateRate;
        }
    };

    class OpenGlovesTrackingTask : public OH::Task<OpenGlovesTrackingTask> {
        friend class OH::Task<OpenGlovesTrackingTask>;

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
          OpenGlovesConfig& config,
          ICommunication& communication,
          HandSensors& fingers,
          std::vector<StringEncodedMemoizedSensor<bool>*>& buttons,
          std::vector<StringEncodedMemoizedSensor<uint16_t>*>& joysticks,
          std::vector<IStringEncodedMemoizedSensor*>& otherSensors,
          std::optional<StringEncodedMemoizedSensor<bool>>& calibrationButton,
          OH::TaskConfig taskConfig
        ) :
          config(config),
          communication(communication),
          fingers(fingers),
          buttons(buttons),
          joysticks(joysticks),
          otherSensors(otherSensors),
          calibrationButton(calibrationButton),
          allSensors(std::vector<IStringEncodedMemoizedSensor*>()),
          OH::Task<OpenGlovesTrackingTask>(taskConfig)
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
            this->OH::Task<OpenGlovesTrackingTask>::begin();
        };

      private:
        OpenGlovesConfig& config;

        HandSensors& fingers;
        ICommunication& communication;
        std::vector<StringEncodedMemoizedSensor<bool>*>& buttons;
        std::vector<StringEncodedMemoizedSensor<uint16_t>*>& joysticks;
        std::vector<IStringEncodedMemoizedSensor*>& otherSensors;

        std::vector<IStringEncodedMemoizedSensor*> allSensors;

        std::optional<StringEncodedMemoizedSensor<bool>>& calibrationButton;
        std::vector<OH::ICalibrated*> calibrated = std::vector<OH::ICalibrated*>();
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
                if (elapsed < this->config.updateInterval) {
                    delay(this->config.updateInterval - elapsed);
                }
            }
        };
    };
} // namespace OpenGloves
