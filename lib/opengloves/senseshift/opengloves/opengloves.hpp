#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <string>

#include <opengloves/opengloves.hpp>

namespace SenseShift::OpenGloves {
    class ITransport : public IInitializable {
      public:
        virtual auto send(const char* buffer, size_t length) -> size_t = 0;
        virtual auto hasData() -> bool = 0;
        virtual auto read(char* buffer, size_t length) -> size_t = 0;
    };
} // namespace SenseShift::OpenGloves
