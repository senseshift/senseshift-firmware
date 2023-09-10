#pragma once

#include <cstddef>
#include <cstdint>

#include <senseshift/buffer.hpp>

#include "og_protocol.hpp"

namespace SenseShift::OpenGloves {
    struct ITransport {
        virtual void setup(){};
        virtual size_t send(const char* buffer, size_t length) = 0;
        virtual bool hasData() = 0;
        virtual size_t read(char* buffer, size_t length) = 0;
    };

    struct IEncoding {
        virtual size_t
          serialize(const std::vector<::OpenGloves::IStringEncodedMemoizedSensor*>& sensors, char* buffer) const = 0;

        virtual bool deserialize(
          const char* buffer, const size_t length, std::map<::OpenGloves::Command, uint16_t>& commands
        ) const = 0;
    };
} // namespace SenseShift::OpenGloves
