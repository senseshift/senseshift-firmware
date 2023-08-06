#include "og_alpha_encoding.hpp"

namespace OpenGloves {
    std::map<Command, uint16_t> AlphaEncodingService::splitCommands(std::string input_string)
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
        size_t split_index = current_command.find_last_not_of(valueSymbols) + 1;

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
} // namespace OpenGloves
