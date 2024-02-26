#include "opengloves/opengloves.hpp"

#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <string>
#include <variant>

namespace og {

    inline auto ifloor(float x) -> int
    {
        return static_cast<int>(x);
    }

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

    auto AlphaEncoder::decode_output(const char* data, size_t length) const -> OutputData
    {
        if (length == 0) {
            return OutputInvalid{};
        }

        const auto commands = split_to_map(data, length);
        if (commands.empty()) {
            return OutputInvalid{};
        }

        // We assume all commands are for ffb, if there is any ffb command
        const auto& thumb_curl = commands.find("A");
        const auto& index_curl = commands.find("B");
        const auto& middle_curl = commands.find("C");
        const auto& ring_curl = commands.find("D");
        const auto& pinky_curl = commands.find("E");

        if (thumb_curl != commands.end() || index_curl != commands.end() || middle_curl != commands.end() ||
            ring_curl != commands.end() || pinky_curl != commands.end()
        ) {
            OutputForceFeedbackData ffb{};

            if (thumb_curl != commands.end()) {
                ffb.thumb = std::stof(thumb_curl->second) / MAX_ANALOG_VALUE;
            }

            if (index_curl != commands.end()) {
                ffb.index = std::stof(index_curl->second) / MAX_ANALOG_VALUE;
            }

            if (middle_curl != commands.end()) {
                ffb.middle = std::stof(middle_curl->second) / MAX_ANALOG_VALUE;
            }

            if (ring_curl != commands.end()) {
                ffb.ring = std::stof(ring_curl->second) / MAX_ANALOG_VALUE;
            }

            if (pinky_curl != commands.end()) {
                ffb.pinky = std::stof(pinky_curl->second) / MAX_ANALOG_VALUE;
            }

            return ffb;
        }

        // const auto& haptics_frequency = commands.find("F");
        // if (haptics_frequency != commands.end()) {
        //     OutputHaptics haptics{};
        //     return haptics;
        // }

        return OutputInvalid{};
    }

    /// Splits the input data into a map of commands and their respective values.
    ///
    /// Example: `A100(AB)200B300(BB)400C500\n` -> `{"A": "100", "(AB)": "200", "B": "300", "(BB)": "400", "C": "500"}`
    auto AlphaEncoder::split_to_map(const char* data, size_t length) const -> std::map<std::string, std::string>
    {
        std::map<std::string, std::string> result{};

        // Start at the beginning of the data
        size_t command_start = 0;
        for (size_t i = 0; i < length; i++) {
            const auto& current_char = data[i];

            // Start a new command if the character is non-numeric or an opening parenthesis
            // and previous character is a numeric character
            const bool is_command_start = ((isdigit(current_char)) == 0) || current_char == '(';
            const bool prev_is_digit = isdigit(data[i - 1]) != 0;
            if (is_command_start && i > 0 && prev_is_digit) {
                split_command(data, command_start, i, result);
                command_start = i;
            }
        }

        // Add the last command
        split_command(data, command_start, length, result);

        return result;
    }

    void AlphaEncoder::split_command(
      const char* data, size_t start, size_t length, std::map<std::string, std::string>& commands
    ) const
    {
        const std::string current_command = std::string(data + start, length - start);

        if (current_command.empty()) {
            return;
        }

        const size_t split_index = current_command.find_first_of("0123456789");

        // If there is no numeric value, the command is empty (likely a binary command)
        if (split_index == std::string::npos) {
            commands[current_command] = "";
            return;
        }

        const std::string command = current_command.substr(0, split_index);
        const std::string value = current_command.substr(split_index, current_command.length() - split_index);

        commands[command] = value;
    }
} // namespace og