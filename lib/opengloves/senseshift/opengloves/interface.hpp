#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include <senseshift/buffer.hpp>

#include "og_protocol.hpp"

namespace SenseShift::OpenGloves {
    struct ITransport {
        virtual void setup(){};
        virtual auto send(const char* buffer, size_t length) -> size_t = 0;
        virtual auto hasData() -> bool = 0;
        virtual auto read(char* buffer, size_t length) -> size_t = 0;
    };

    struct IEncoding {
        /// \param [in] sensors
        /// \param [out] buffer
        virtual auto serialize(
          const std::vector<::OpenGloves::IStringEncodedMemoizedSensor*>& sensors,
          char* buffer
        ) const -> size_t = 0;

        /// \param [in] buffer
        /// \param [in] length
        /// \param [out] commands
        virtual auto deserialize(
          const char* buffer,
          size_t length,
          std::map<::OpenGloves::Command, uint16_t>& commands
        ) const -> bool = 0;
    };
} // namespace SenseShift::OpenGloves
