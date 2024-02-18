#pragma once

#include <cstddef>
#include <set>
#include <variant>
#include <vector>

#include <opengloves/opengloves.hpp>

#include <senseshift/core/component.hpp>
#include <senseshift/input/sensor.hpp>
#include <senseshift/output/output.hpp>

namespace SenseShift::OpenGloves {
    class ITransport : public IInitializable {
      public:
        virtual auto send(const char* buffer, size_t length) -> size_t = 0;
        virtual auto hasData() -> bool = 0;
        virtual auto read(char* buffer, size_t length) -> size_t = 0;
    };

    using FloatSensor = ::SenseShift::Input::FloatSensor;
    using BinarySensor = ::SenseShift::Input::BinarySensor;

    class InputSensors :
      public og::InputPeripheral<FloatSensor*, BinarySensor*>,
      public Component,
      public ::SenseShift::Input::Calibration::ICalibrated {
      public:
        void init() override
        {
            for (auto& finger_curl : this->curl.fingers) {
                for (auto& joint_sensor : finger_curl.curl) {
                    if (joint_sensor != nullptr) {
                        joint_sensor->init();
                        this->calibrated_inputs_.insert(joint_sensor);
                    }
                }
            }

            for (auto& finger_splay : this->splay.fingers) {
                if (finger_splay != nullptr) {
                    finger_splay->init();
                    this->calibrated_inputs_.insert(finger_splay);
                }
            }

            SS_INIT_NOT_NULL(this->joystick.x);
            SS_INIT_NOT_NULL(this->joystick.y);
            SS_INIT_NOT_NULL(this->joystick.press);

            for (auto& button : this->buttons) {
                SS_INIT_NOT_NULL(button.press);
            }

            for (auto& analog_button : this->analog_buttons) {
                SS_INIT_NOT_NULL(analog_button.press);
                SS_INIT_NOT_NULL(analog_button.value);
            }
        }

        void tick() override
        {
            for (auto& finger_curl : this->curl.fingers) {
                for (auto& joint_sensor : finger_curl.curl) {
                    SS_TICK_NOT_NULL(joint_sensor);
                }
            }

            for (auto& finger_splay : this->splay.fingers) {
                SS_TICK_NOT_NULL(finger_splay);
            }

            SS_TICK_NOT_NULL(this->joystick.x);
            SS_TICK_NOT_NULL(this->joystick.y);
            SS_TICK_NOT_NULL(this->joystick.press);

            for (auto& button : this->buttons) {
                SS_TICK_NOT_NULL(button.press);
            }

            for (auto& analog_button : this->analog_buttons) {
                SS_TICK_NOT_NULL(analog_button.press);
                SS_TICK_NOT_NULL(analog_button.value);
            }
        }

        auto collectData() -> og::InputPeripheralData
        {
            og::InputPeripheralData data{};

            const auto& curls = this->curl.fingers;
            const auto& splays = this->splay.fingers;
            for (auto i = 0; i < curls.size(); i++) {
                const auto& finger_curl = curls[i].curl;
                for (auto j = 0; j < finger_curl.size(); j++) {
                    auto* joint_sensor = finger_curl[j];
                    if (joint_sensor != nullptr) {
                        data.curl.fingers[i].curl[j] = joint_sensor->getValue();
                    }
                }

                auto* finger_splay = splays[i];
                if (finger_splay != nullptr) {
                    data.splay.fingers[i] = finger_splay->getValue();
                }
            }

            if (this->joystick.x != nullptr) {
                data.joystick.x = this->joystick.x->getValue();
            }
            if (this->joystick.y != nullptr) {
                data.joystick.y = this->joystick.y->getValue();
            }
            if (this->joystick.press != nullptr) {
                data.joystick.press = this->joystick.press->getValue();
            }

            for (auto i = 0; i < this->buttons.size(); i++) {
                auto* button = this->buttons[i].press;
                if (button != nullptr) {
                    data.buttons[i].press = button->getValue();
                }
            }

            for (auto i = 0; i < this->analog_buttons.size(); i++) {
                auto* button = this->analog_buttons[i].press;
                if (button != nullptr) {
                    data.analog_buttons[i].press = button->getValue();
                }
                auto* value = this->analog_buttons[i].value;
                if (value != nullptr) {
                    data.analog_buttons[i].value = value->getValue();
                }
            }

            return data;
        }

        void reselCalibration() override
        {
            for (const auto& calibrated_input : this->calibrated_inputs_) {
                calibrated_input->reselCalibration();
            }
        }

        void startCalibration() override
        {
            for (const auto& calibrated_input : this->calibrated_inputs_) {
                calibrated_input->startCalibration();
            }
        }

        void stopCalibration() override
        {
            for (const auto& calibrated_input : this->calibrated_inputs_) {
                calibrated_input->stopCalibration();
            }
        }

      private:
        std::set<FloatSensor*> calibrated_inputs_{};
    };

    using FloatOutput = ::SenseShift::Output::IFloatOutput;

    class OutputWriters : public IInitializable {
      public:
        og::OutputForceFeedback<FloatOutput*, void*> ffb;

        void init() override
        {
            for (auto& finger : this->ffb.fingers) {
                if (finger != nullptr) {
                    finger->init();
                }
            }
        }

        void apply(const og::OutputData& data)
        {
            if (std::holds_alternative<og::OutputForceFeedbackData>(data)) {
                const auto& ffb_data = std::get<og::OutputForceFeedbackData>(data);
                for (auto i = 0; i < this->ffb.fingers.size(); i++) {
                    auto* finger = this->ffb.fingers[i];
                    if (finger != nullptr) {
                        finger->writeState(ffb_data.fingers[i]);
                    }
                }
            }
        }
    };
} // namespace SenseShift::OpenGloves
