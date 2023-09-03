#include "senseshift/opengloves/encoding/alpha.hpp"

#include <string.h>

namespace SenseShift::OpenGloves {
    const std::map<AlphaEncodingService::Command, uint16_t>
      AlphaEncodingService::deserialize(const std::string& input_string) const
    {
        std::map<Command, uint16_t> commands;

        size_t start = 0; // Start of the current command
        for (size_t i = 0; i < input_string.size(); i++) {
            char ch = input_string[i];

            // Start a new command if the character is non-numeric or an opening parenthesis
            // and previous character is a numeric character
            if ((!(isdigit(ch)) || ch == '(') && i > 0 && isdigit(input_string[i - 1])) {
                AlphaEncodingService::splitCommand(input_string, start, i, commands);
                start = i;
            }
        }

        AlphaEncodingService::splitCommand(input_string, start, input_string.size(), commands);

        return commands;
    }

    void AlphaEncodingService::splitCommand(
      const std::string& input_string, size_t start, size_t end, std::map<Command, uint16_t>& commands
    )
    {
        std::string current_command = input_string.substr(start, end - start);

        if (current_command.empty()) {
            return;
        }

        // Split the command into prefix and number
        size_t split_index = current_command.find_last_not_of(valueSymbols.data()) + 1;

        if (split_index >= current_command.size()) {
            log_w("Invalid command: %s", current_command.c_str());
            return;
        }

        std::string prefix = current_command.substr(0, split_index);
        int number = std::stoi(current_command.substr(split_index));

        // Check if the command prefix is in commandMap
        auto it = commandMap.find(prefix);
        if (it == commandMap.end()) {
            log_w("Unknown command prefix: %s", prefix.c_str());
            return;
        }

        // Look up the Command enum value for the prefix in the commandMap
        Command command = it->second;
        commands[command] = number;
    }

    const std::string
      AlphaEncodingService::serialize(const std::vector<::OpenGloves::IStringEncodedMemoizedSensor*>& sensors)
    {
        memset(this->writeBuffer, 0, 256);
        this->writeBuffer[0] = '\0';

        size_t offset = 0;

        for (size_t i = 0; i < sensors.size(); i++) {
            // The offset is the total charecters already added to the string.
            offset += sensors[i]->encodeString(this->writeBuffer + offset);
        }

        // Add a newline and terminator to the end of the encoded string.
        this->writeBuffer[offset++] = '\n';
        this->writeBuffer[offset] = '\0';

        return std::string(this->writeBuffer, offset);
    }
} // namespace SenseShift::OpenGloves
