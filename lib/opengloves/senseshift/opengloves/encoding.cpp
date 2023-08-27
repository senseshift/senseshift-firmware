#include "senseshift/opengloves/encoding.hpp"

namespace SenseShift::OpenGloves {
    const std::vector<Command_t> AlphaEncodingService::decode(const std::string& buffer)
    {
        std::vector<Command_t> commands;

        size_t start = 0; // Start of the current command
        for (size_t i = 0; i < buffer.size(); i++) {
            char ch = buffer[i];

            // Start a new command if the character is non-numeric or an opening parenthesis
            // and previous character is a numeric character
            if ((!(isdigit(ch)) || ch == '(') && i > 0 && isdigit(buffer[i - 1])) {
                AlphaEncodingService::splitCommand(buffer, start, i, commands);
                start = i;
            }
        }

        AlphaEncodingService::splitCommand(buffer, start, buffer.size(), commands);

        return commands;
    }

    void AlphaEncodingService::splitCommand(
      const std::string& buffer, size_t start, size_t end, std::vector<Command_t>& commands
    )
    {
        std::string current_command = buffer.substr(start, end - start);
        if (current_command.empty()) {
            return;
        }

        // Split the command into prefix and number
        size_t split_index = current_command.find_last_not_of(valueSymbols) + 1;
        std::string prefix = current_command.substr(0, split_index);

        // Check if the command prefix is in commandMap
        auto it = commandMap.find(prefix);
        if (it == commandMap.end()) {
            log_w("Unknown command prefix: %s", prefix.c_str());
            return;
        }

        // Look up the Command enum value for the prefix in the commandMap
        Sensor_t command = it->second;

        if (std::holds_alternative<DigitalSensorType_t>(command)) {
            commands.push_back(std::get<DigitalSensorType_t>(command));
        } else {
            if (split_index >= current_command.size()) {
                log_w("Invalid command: %s", current_command.c_str());
                return;
            }

            int number = std::stoi(current_command.substr(split_index));

            if (std::holds_alternative<Joint_t>(command)) {
                commands.push_back(std::make_pair(std::get<Joint_t>(command), number));
            } else if (std::holds_alternative<AnalogSensorType_t>(command)) {
                commands.push_back(std::make_pair(std::get<AnalogSensorType_t>(command), number));
            } else {
                log_w("Unknown command type: %s", current_command.c_str());
                return;
            }
        }
    }
} // namespace SenseShift::OpenGloves
