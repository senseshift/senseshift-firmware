#include "opengloves/opengloves.hpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <variant>

namespace og {

#ifdef OG_ENCODE_FASTER
    inline auto ifloor(float x) -> int
    {
        union Cast {
            double d;
            long l;
        };
        volatile Cast c;
        c.d = d + 6755399441055743.5;
        return c.l;
    }
#elifdef OG_ENCODE_FAST
    /// Source: https://stackoverflow.com/questions/429632/429812#429812
    inline int float2int(double d)
    {
        union Cast {
            double d;
            long l;
        };
        volatile Cast c;
        c.d = d + 6755399441055744;
        return c.l;
    }

    inline auto ifloor(float x) -> int
    {
        return float2int(x) - (x < float2int(x));
    }
#else
    inline auto ifloor(float x) -> int
    {
        return static_cast<int>(std::floor(x));
    }
#endif

    auto AlphaEncoder::encode_input(const InputData& input, char* buffer, size_t length) const -> size_t
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

            const auto& curls = peripheral.curl.fingers;
            const auto& splays = peripheral.splay.fingers;
#ifdef OG_ENCODE_FAST
            for (auto i = 0; i < curls.size(); i++) {
                const auto& finger = curls[i];
                const auto& finger_curl = finger.curl_total;

                written +=
                  snprintf(buffer + written, length - written, "%c%u", 'A' + i, ifloor(finger_curl * MAX_ANALOG_VALUE));
            }
#else
            for (auto i = 0; i < curls.size(); i++) {
                const auto& finger_curl = curls[i];
                const auto& finger_splay = splays[i];
                const auto finger_alpha_key = 'A' + i;

                written += snprintf(
                  buffer + written,
                  length - written,
                  "%c%u",
                  finger_alpha_key,
                  ifloor(finger_curl.curl_total * MAX_ANALOG_VALUE)
                );

                if (finger_splay > 0.0F) {
                    written += snprintf(
                      buffer + written,
                      length - written,
                      "(%cB)%u",
                      finger_alpha_key,
                      ifloor(finger_splay * MAX_ANALOG_VALUE)
                    );
                }

                const auto& joints = finger_curl.curl;
                for (auto j = 1; j < joints.size(); j++) {
                    const auto& joint = joints[j];
                    const auto joint_alpha_key = 'A' + j;

                    if (joint == 0.0F) {
                        continue;
                    }

                    written += snprintf(
                      buffer + written,
                      length - written,
                      "(%cA%c)%u",
                      finger_alpha_key,
                      joint_alpha_key,
                      ifloor(joint * MAX_ANALOG_VALUE)
                    );
                }
            }
#endif
            if (peripheral.joystick.x != 0.0F) {
                written +=
                  snprintf(buffer + written, length - written, "F%u", ifloor(peripheral.joystick.x * MAX_ANALOG_VALUE));
            }
            if (peripheral.joystick.y != 0.0F) {
                written +=
                  snprintf(buffer + written, length - written, "G%u", ifloor(peripheral.joystick.y * MAX_ANALOG_VALUE));
            }
            if (peripheral.joystick.press) {
                written += snprintf(buffer + written, length - written, "H");
            }

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