#include <map>
#include <stdint.h>
#include <string>
#include <vector>

#include <logging.hpp>

namespace OpenGloves {
    typedef enum Command {
        ThumbCurl,
        ThumbSplay,

        IndexCurl,
        IndexSplay,

        MiddleCurl,
        MiddleSplay,

        RingCurl,
        RingSplay,

        PinkyCurl,
        PinkySplay,
    } Command;

    class AlphaEncodingService {
      public:
        inline static const std::string valueSymbols = "0123456789";
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

        static std::map<Command, int> splitCommands(std::string input_string);

      private:
        static void splitCommand(const std::string& input_string, int start, int end, std::map<Command, int>& commands);
    };
} // namespace OpenGloves
