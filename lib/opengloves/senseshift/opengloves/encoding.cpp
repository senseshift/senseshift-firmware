#include "senseshift/opengloves/encoding.hpp"

#include <senseshift/logging.hpp>

namespace SenseShift::OpenGloves {
    std::vector<Command_t> AlphaEncodingService::deserialize(const std::string& buffer) const
    {
        std::vector<Command_t> commands;

        size_t start = 0; // Start of the current command
        for (size_t i = 0; i < buffer.size(); i++) {
            char ch = buffer[i];

            // Start a new command if the character is non-numeric or an opening parenthesis
            // and previous character is a numeric character
            if ((!(isdigit(ch)) || ch == '(') && i > 0 && isdigit(buffer[i - 1])) {
                std::string command = buffer.substr(start, i - start);
                AlphaEncodingService::parseCommand(buffer.substr(start, i - start), commands);
                start = i;
            }
        }

        AlphaEncodingService::parseCommand(buffer.substr(start), commands);

        return commands;
    }

    std::string AlphaEncodingService::serialize(const std::vector<Command_t>& commands) const
    {
        std::string buffer;

        for (const auto& command : commands) {
            // Look up the command prefix in the commandToPrefixMap
            if (std::holds_alternative<JointCommand_t>(command)) {
                auto jointCommand = std::get<JointCommand_t>(command);
                auto it = AlphaEncodingService::commandToPrefixMap.find(std::get<Joint>(jointCommand));
                if (it == AlphaEncodingService::commandToPrefixMap.end()) {
                    log_w("Unknown joint command: %d", std::get<Joint>(jointCommand));
                    continue;
                }

                auto value = std::get<AnalogSensorValue_t>(jointCommand);
                if (value < ANALOG_SENSOR_VALUE_MIN || value > ANALOG_SENSOR_VALUE_MAX) {
                    log_w("Invalid joint command value: %d", value);
                    continue;
                }

                buffer += it->second.data();
                buffer += std::to_string(value);
            } else if (std::holds_alternative<AnalogSensorCommand_t>(command)) {
                auto analogSensorCommand = std::get<AnalogSensorCommand_t>(command);
                auto it =
                  AlphaEncodingService::commandToPrefixMap.find(std::get<AnalogSensorType>(analogSensorCommand));
                if (it == AlphaEncodingService::commandToPrefixMap.end()) {
                    log_w("Unknown analog sensor command: %d", std::get<AnalogSensorType>(analogSensorCommand));
                    continue;
                }

                auto value = std::get<AnalogSensorValue_t>(analogSensorCommand);
                if (value < ANALOG_SENSOR_VALUE_MIN || value > ANALOG_SENSOR_VALUE_MAX) {
                    log_w("Invalid analog sensor command value: %d", value);
                    continue;
                }

                buffer += it->second.data();
                buffer += std::to_string(value);
            } else if (std::holds_alternative<DigitalSensorType>(command)) {
                auto it = AlphaEncodingService::commandToPrefixMap.find(std::get<DigitalSensorType>(command));
                if (it == AlphaEncodingService::commandToPrefixMap.end()) {
                    log_w("Unknown digital sensor command: %d", std::get<DigitalSensorType>(command));
                    continue;
                }

                buffer += it->second.data();
            } else {
                log_w("Unknown command type");
                continue;
            }
        }

        return buffer;
    }

    void AlphaEncodingService::parseCommand(const std::string& current, std::vector<Command_t>& dest)
    {
        if (current.empty()) {
            return;
        }

        // Split the command into prefix and number
        size_t split_index = current.find_last_not_of(valueSymbols.data()) + 1;
        std::string prefix = current.substr(0, split_index);

        // Check if the command prefix is in commandMap
        auto it = AlphaEncodingService::prefixToCommandMap.find(prefix);
        if (it == AlphaEncodingService::prefixToCommandMap.end()) {
            log_w("Unknown command prefix: %s", prefix.c_str());
            return;
        }

        // Look up the Command enum value for the prefix in the commandMap
        Sensor_t command = it->second;

        if (std::holds_alternative<DigitalSensorType>(command)) {
            dest.push_back(std::get<DigitalSensorType>(command));
        } else {
            if (split_index >= current.size()) {
                log_w("Invalid command: %s", current.c_str());
                return;
            }

            // Parse the number
            int number = 0;
            try {
                number = std::stoi(current.substr(split_index));
            } catch (std::invalid_argument) {
                log_w("Invalid command: %s", current.c_str());
                return;
            }

            if (std::holds_alternative<Joint>(command)) {
                dest.push_back(std::make_pair(std::get<Joint>(command), number));
            } else if (std::holds_alternative<AnalogSensorType>(command)) {
                dest.push_back(std::make_pair(std::get<AnalogSensorType>(command), number));
            } else {
                log_w("Unknown command type: %s", current.c_str());
                return;
            }
        }
    }
} // namespace SenseShift::OpenGloves
