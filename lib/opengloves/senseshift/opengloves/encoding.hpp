#pragma once

#include <senseshift/opengloves/interface.hpp>

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

namespace SenseShift::OpenGloves {
    class AlphaEncodingService {
      public:
        inline static const std::basic_string valueSymbols = "0123456789";
        inline static const auto commandMap = frozen::make_map<std::string, Sensor_t>({
          // clang-format off
          { "A", Joint::ThumbCurl },
          { "(AB)", Joint::ThumbSplay },
          { "B", Joint::IndexCurl },
          { "(BB)", Joint::IndexSplay },
          { "C", Joint::MiddleCurl },
          { "(CB)", Joint::MiddleSplay },
          { "D", Joint::RingCurl },
          { "(DB)", Joint::RingSplay },
          { "E", Joint::LittleCurl },
          { "(EB)", Joint::LittleSplay },
          { "I", DigitalSensorType::ButtonCalibrate },
          // clang-format on
        });

        AlphaEncodingService() = default;

        static const std::vector<Command_t> decode(const std::string& buffer);
        // static const std::string encode(const std::vector<Command_t>& commands);

      private:
        static void splitCommand(const std::string& buffer, size_t start, size_t end, std::vector<Command_t>& dest);
    };
} // namespace SenseShift::OpenGloves
