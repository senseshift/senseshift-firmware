#pragma once

#include <cstddef>
#include <cstdint>

#include "og_protocol.hpp"

namespace SenseShift::OpenGloves {
    struct ITransport {
        virtual void setup(){};
        virtual size_t send(const char* buffer, size_t length) = 0;
        virtual bool hasData() = 0;
        virtual size_t read(char* buffer, size_t length) = 0;
    };

    struct IEncoding {
        virtual const std::string serialize(const std::vector<::OpenGloves::IStringEncodedMemoizedSensor*>& sensors
        ) = 0;
        virtual const std::map<::OpenGloves::Command, uint16_t> deserialize(const std::string& buffer) const = 0;
    };
} // namespace SenseShift::OpenGloves
