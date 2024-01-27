#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <string>
#include <tuple>

#include <senseshift/body/haptics/interface.hpp>
#include <senseshift/body/haptics/body.hpp>

namespace SenseShift::BH {
    class Decoder {
      public:
        using VibroEffectData = ::SenseShift::Body::Haptics::VibroEffectData;

        using Effect = ::SenseShift::Body::Haptics::Effect;
        using Target = ::SenseShift::Body::Haptics::Target;
        using Position = ::SenseShift::Body::Haptics::Position;

        using OutputLayout = std::tuple<Target, Position>;

        static constexpr size_t VEST_LAYOUT_SIZE = 40;
        static constexpr size_t VEST_PAYLOAD_SIZE = 20;

        template<size_t N>
        static void applyPlain(
          FloatBody* output,
          const std::array<std::uint8_t, N>& value,
          const std::array<OutputLayout, N>& layout,
          const Effect effect
        ) {
            for (size_t i = 0; i < N; i++) {
                const auto [target, position] = layout[i];
                const std::uint8_t byte = value[i];

                output->effect(
                  target,
                  position,
                  static_cast<FloatBody::Plane::Value>(effectDataFromByte(byte))
                );
            }
        }

        template<size_t N>
        static void applyPlain(
          FloatBody* output,
          std::string& value,
          const std::array<OutputLayout, N>& layout,
          const Effect effect
        ) {
            std::array<std::uint8_t, N> buf{};
            std::size_t copyLength = std::min(value.size(), sizeof(buf));
            std::memcpy(buf.data(), value.c_str(), copyLength);

            applyPlain(output, buf, layout, effect);
        }

        /**
         * Apply plain-encoded data to the output.
         */
        template<size_t N>
        static void applyPlain(
          FloatBody* output,
          const std::array<std::uint8_t, N>& value,
          const std::array<Position, N>& layout,
          const Effect effect,
          const Target target
        ) {
            for (size_t i = 0; i < N; i++) {
                const auto position = layout[i];
                const uint8_t byte = value[i];

                output->effect(
                  target,
                  position,
                  static_cast<FloatBody::Plane::Value>(effectDataFromByte(byte))
                );
            }
        }

        template<size_t N>
        static void applyPlain(
          FloatBody* output,
          std::string& value,
          const std::array<Position, N>& layout,
          const Effect effect,
          const Target target
        ) {
            std::array<std::uint8_t, N> buf{};
            std::size_t copyLength = std::min(value.size(), sizeof(buf));
            std::memcpy(buf.data(), value.c_str(), copyLength);

            applyPlain(output, buf, layout, effect, target);
        }

        /**
         * Apply vest-encoded data to the output.
         */
        static void applyVest(
          FloatBody* output,
          const std::array<uint8_t, VEST_PAYLOAD_SIZE>& value,
          const std::array<OutputLayout, VEST_LAYOUT_SIZE>& layout
        ) {
            for (size_t i = 0; i < VEST_PAYLOAD_SIZE; i++) {
                const std::uint8_t byte = value[i];
                const size_t actIndex = i * 2;

                output->effect(
                  std::get<0>(layout[actIndex]),
                  std::get<1>(layout[actIndex]),
                  static_cast<FloatBody::Plane::Value>(effectDataFromByte(((byte >> 4) & 0xf), 15))
                );
                output->effect(
                  std::get<0>(layout[actIndex + 1]),
                  std::get<1>(layout[actIndex + 1]),
                  static_cast<FloatBody::Plane::Value>(effectDataFromByte((byte & 0xf), 15).getIntensity())
                );
            }
        }

        static void applyVest(
          FloatBody* output,
          std::string& value,
          const std::array<OutputLayout, VEST_LAYOUT_SIZE>& layout
        ) {
            std::array<std::uint8_t, VEST_PAYLOAD_SIZE> buf{};
            const size_t copyLength = std::min(value.size(), sizeof(buf));
            std::memcpy(buf.data(), value.c_str(), copyLength);

            applyVest(output, buf, layout);
        }

        /**
         * Apply grouped vest-encoded data to the output.
         */
        template<size_t N>
        static void applyVestGrouped(
          FloatBody* output,
          const std::array<std::uint8_t, VEST_PAYLOAD_SIZE>& value,
          const std::array<OutputLayout, VEST_LAYOUT_SIZE>& layout,
          const std::array<std::uint8_t, N>& layoutGroups
        ) {
            std::array<std::uint8_t, VEST_LAYOUT_SIZE> result{};

            // Unpack values
            for (auto i = 0; i < VEST_PAYLOAD_SIZE; i++) {
                const std::uint8_t byte = value[i];
                const size_t actIndex = i * 2;

                result[actIndex] = (byte >> 4) & 0xf;
                result[actIndex + 1] = (byte & 0xf);
            }

            // Assign max value into each group
            for (auto i = 0; i < N; i++) {
                auto groupIndex = layoutGroups[i];

                if (groupIndex % 10 >= 4) {
                    // Top 3 rows of x40
                    auto maxValue = std::max({ result[groupIndex], result[groupIndex + 2], result[groupIndex + 4] });

                    result[groupIndex] = maxValue;
                    result[groupIndex + 2] = maxValue;
                    result[groupIndex + 4] = maxValue;
                } else {
                    // Bottom 2 rows of x40
                    auto maxValue = std::max({ result[groupIndex], result[groupIndex + 2] });

                    result[groupIndex] = maxValue;
                    result[groupIndex + 2] = maxValue;
                }
            }

            for (uint8_t i = 0; i < VEST_LAYOUT_SIZE; i++) {
                // take only meaningful values
                if (std::find(layoutGroups.begin(), layoutGroups.end(), i) == layoutGroups.end()) {
                    continue;
                }

                const auto target = std::get<0>(layout[i]);
                const auto position = std::get<1>(layout[i]);

                output->effect(
                  target,
                  position,
                  static_cast<FloatBody::Plane::Value>(effectDataFromByte(result[i], 15))
                );
            }
        }

        template<size_t N>
        static void applyVestGrouped(
          FloatBody* output,
          std::string& value,
          const std::array<OutputLayout, VEST_LAYOUT_SIZE>& layout,
          const std::array<std::uint8_t, N>& layoutGroups
        ) {
            std::array<std::uint8_t, VEST_PAYLOAD_SIZE> buf{};
            const size_t copyLength = std::min(value.size(), sizeof(buf));
            std::memcpy(buf.data(), value.c_str(), copyLength);

            applyVestGrouped(output, buf, layout, layoutGroups);
        }

      private:
        static auto effectDataFromByte(const uint8_t byte, const uint8_t maxValue = 100) -> VibroEffectData
        {
            const auto value = static_cast<float>(byte) / static_cast<float>(maxValue);
            return VibroEffectData(value);
        }
    };
} // namespace SenseShift::BH
