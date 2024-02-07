#include "opengloves/opengloves.hpp"

#include <cstdint>
#include <cstdio>
#include <variant>

namespace og {

    auto AlphaEncoder::encode_input(const Input& input, char* buffer, size_t length) const -> size_t
    {
        if (std::holds_alternative<InputInfoData>(input)) {
            const auto& info = std::get<InputInfoData>(input);

            const auto& keyFirmwareVersion = AlphaEncoder::INFO_FIRMWARE_VERSION_KEY;
            const auto& keyDeviceType = AlphaEncoder::INFO_DEVICE_TYPE_KEY;
            const auto& keyHand = AlphaEncoder::INFO_HAND_KEY;

            return snprintf(
              buffer,
              length,
              "%s%u%s%u%s%u\n",
              keyFirmwareVersion,
              info.firmware_version,
              keyDeviceType,
              info.device_type,
              keyHand,
              info.hand
            );
        }

        if (std::holds_alternative<InputPeripheralData>(input)) {
            const auto peripheral = std::get<InputPeripheralData>(input);
            auto written = 0;

            const auto& curl = peripheral.curl.fingers;
#ifdef OG_ENCODE_FAST
            for (auto i = 0; i < curl.size(); i++) {
                const auto& finger = curl[i];
                const auto& finger_curl = finger.curl_total;

                written +=
                  snprintf(buffer + written, length - written, "%c%.0f", 'A' + i, finger_curl * MAX_ANALOG_VALUE);
            }
#else
            for (auto i = 0; i < curl.size(); i++) {
                const auto& finger = curl[i];
                const auto& joints = finger.curl;

                for (auto j = 0; j < joints.size(); j++) {
                    const auto& joint = joints[j];

                    // skip if joint is 0.0, except total_curl (it is probably not enabled)
                    if (joint == 0.0F && j != 0) {
                        continue;
                    }

                    const auto& jointKey = AlphaEncoder::FINGER_CURL_JOINT_ALPHA_KEY[i][j];

                    written +=
                      snprintf(buffer + written, length - written, "%s%.0f", jointKey.data(), joint * MAX_ANALOG_VALUE);
                }
            }

            const auto& splay = peripheral.splay.fingers;
            for (auto i = 0; i < splay.size(); i++) {
                const auto& finger = splay[i];

                if (finger == 0.0F) {
                    continue;
                }

                written += snprintf(
                  buffer + written,
                  length - written,
                  "(%cB)%.0f",
                  AlphaEncoder::FINGER_ALPHA_KEY[i],
                  finger * MAX_ANALOG_VALUE
                );
            }
#endif

            const auto& buttons = peripheral.buttons;
            for (auto i = 0; i < buttons.size(); i++) {
                const auto& button = buttons[i];
                if (button.press) {
                    const auto& buttonKey = AlphaEncoder::BUTTON_ALPHA_KEY[i];
                    written += snprintf(buffer + written, length - written, "%c", buttonKey);
                }
            }

            const auto& analog_buttons = peripheral.analog_buttons;
            for (auto i = 0; i < analog_buttons.size(); i++) {
                const auto& button = analog_buttons[i];
                if (button.press) {
                    const auto& buttonKey = AlphaEncoder::ANALOG_BUTTON_ALPHA_KEY[i];
                    written += snprintf(buffer + written, length - written, "%c", buttonKey);
                }
            }

            // Add newline and null terminator
            written += snprintf(buffer + written, length - written, "\n");

            return written;
        }

        return 0;
    }

    auto AlphaEncoder::parse_output(const char* data, size_t length) const -> Output
    {
        return {};
    }
} // namespace og