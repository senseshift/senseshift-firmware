#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>

#include <og_protocol.hpp>
#include <senseshift/logging.hpp>

namespace OpenGloves {
    class AlphaEncodingService {
      public:
        inline static const std::string valueSymbols = "0123456789";
        // TODO: use Frozen library to save memory
        inline static const std::map<std::string, Command> commandMap = {
            // clang-format off
            { "A", Command::ThumbCurl },
            { "(AB)", Command::ThumbSplay },
            { "B", Command::IndexCurl },
            { "(BB)", Command::IndexSplay },
            { "C", Command::MiddleCurl },
            { "(CB)", Command::MiddleSplay },
            { "D", Command::RingCurl },
            { "(DB)", Command::RingSplay },
            { "E", Command::PinkyCurl },
            { "(EB)", Command::PinkySplay },
            // clang-format on
        };

        AlphaEncodingService() = default;

        static std::map<Command, uint16_t> splitCommands(std::string input_string);

      private:
        CommandCallback callback = nullptr;

        static void splitCommand(
          const std::string& input_string, size_t start, size_t end, std::map<Command, uint16_t>& commands
        );
    };
} // namespace OpenGloves
