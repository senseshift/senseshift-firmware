#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>

#include <frozen/map.h>
#include <frozen/string.h>
#include <frozen/unordered_map.h>

#include <og_protocol.hpp>
#include <senseshift/logging.hpp>

namespace OpenGloves {
    class AlphaEncodingService {
      public:
        inline static constexpr frozen::string valueSymbols = "0123456789";
        inline static const auto commandMap = frozen::make_map<std::string, Command>({
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
        });

        AlphaEncodingService() = default;

        static std::map<Command, uint16_t> splitCommands(std::string input_string);

      private:
        CommandCallback callback = nullptr;

        static void splitCommand(
          const std::string& input_string, size_t start, size_t end, std::map<Command, uint16_t>& commands
        );
    };
} // namespace OpenGloves
