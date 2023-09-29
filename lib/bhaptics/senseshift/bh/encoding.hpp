#pragma once

#include <cstring>
#include <senseshift/body/haptics/body.hpp>

namespace SenseShift::BH {
    class Decoder {
      public:
        using VibroEffectData = ::SenseShift::Body::Haptics::VibroEffectData;
        using EffectData = ::SenseShift::Body::Haptics::EffectData;

        using Effect = ::SenseShift::Body::Haptics::Effect;
        using Target = ::SenseShift::Body::Haptics::Target;
        using Position = ::SenseShift::Body::Haptics::Position;

        using HapticBody = ::SenseShift::Body::Haptics::HapticBody;

        using OutputLayout = std::tuple<Target, Position>;

        static constexpr size_t VEST_LAYOUT_SIZE = 40;
        static constexpr size_t VEST_PAYLOAD_SIZE = 20;

        template<size_t N>
        static void applyPlain(
          HapticBody* output, const uint8_t (&value)[N], const OutputLayout (&layout)[N], const Effect effect
        )
        {
            for (size_t i = 0; i < N; i++) {
                const auto [target, position] = layout[i];
                const uint8_t byte = value[i];

                output->effect({
                  .effect = effect,
                  .target = target,
                  .position = position,
                  .data = effectDataFromByte(effect, byte),
                });
            }
        }

        template<size_t N>
        static void
          applyPlain(HapticBody* output, std::string& value, const OutputLayout (&layout)[N], const Effect effect)
        {
            std::uint8_t buf[N];
            std::size_t copyLength = std::min(value.size(), sizeof(buf));
            std::memcpy(buf, value.c_str(), copyLength);

            applyPlain(output, buf, layout, effect);
        }

        /**
         * Apply plain-encoded data to the output.
         */
        template<size_t N>
        static void applyPlain(
          HapticBody* output,
          const uint8_t (&value)[N],
          const Position (&layout)[N],
          const Effect effect,
          const Target target
        )
        {
            for (size_t i = 0; i < N; i++) {
                const auto position = layout[i];
                const uint8_t byte = value[i];

                output->effect({
                  .effect = effect,
                  .target = target,
                  .position = position,
                  .data = effectDataFromByte(effect, byte),
                });
            }
        }

        template<size_t N>
        static void applyPlain(
          HapticBody* output, std::string& value, const Position (&layout)[N], const Effect effect, const Target target
        )
        {
            std::uint8_t buf[N];
            std::size_t copyLength = std::min(value.size(), sizeof(buf));
            std::memcpy(buf, value.c_str(), copyLength);

            applyPlain(output, buf, layout, effect, target);
        }

        /**
         * Apply vest-encoded data to the output.
         */
        static void applyVest(
          HapticBody* output, const uint8_t (&value)[VEST_PAYLOAD_SIZE], const OutputLayout (&layout)[VEST_LAYOUT_SIZE]
        )
        {
            for (size_t i = 0; i < VEST_PAYLOAD_SIZE; i++) {
                uint8_t byte = value[i];
                uint actIndex = i * 2;
                output->effect({
                  .effect = Effect::Vibro,
                  .target = std::get<0>(layout[actIndex]),
                  .position = std::get<1>(layout[actIndex]),
                  .data = effectDataFromByte(Effect::Vibro, ((byte >> 4) & 0xf), 15),
                });
                output->effect({
                  .effect = Effect::Vibro,
                  .target = std::get<0>(layout[actIndex + 1]),
                  .position = std::get<1>(layout[actIndex + 1]),
                  .data = effectDataFromByte(Effect::Vibro, (byte & 0xf), 15),
                });
            }
        }

        static void applyVest(HapticBody* output, std::string& value, const OutputLayout (&layout)[VEST_LAYOUT_SIZE])
        {
            std::uint8_t buf[VEST_PAYLOAD_SIZE];
            std::size_t copyLength = std::min(value.size(), sizeof(buf));
            std::memcpy(buf, value.c_str(), copyLength);

            applyVest(output, buf, layout);
        }

        /**
         * Apply grouped vest-encoded data to the output.
         */
        template<size_t N>
        static void applyVestGrouped(
          HapticBody* output,
          const uint8_t (&value)[VEST_PAYLOAD_SIZE],
          const OutputLayout (&layout)[VEST_LAYOUT_SIZE],
          const uint8_t (&layoutGroups)[N]
        )
        {
            uint8_t result[VEST_LAYOUT_SIZE];

            // Unpack values
            for (auto i = 0; i < VEST_PAYLOAD_SIZE; i++) {
                uint8_t byte = value[i];
                uint actIndex = i * 2;

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
                if (!::SenseShift::contains(layoutGroups, VEST_LAYOUT_SIZE, i)) {
                    continue;
                }

                const auto target = std::get<0>(layout[i]);
                const auto position = std::get<1>(layout[i]);

                output->effect({
                  .effect = Effect::Vibro,
                  .target = target,
                  .position = position,
                  .data = effectDataFromByte(Effect::Vibro, result[i], 15),
                });
            }
        }

        template<size_t N>
        static void applyVestGrouped(
          HapticBody* output,
          std::string& value,
          const OutputLayout (&layout)[VEST_LAYOUT_SIZE],
          const uint8_t (&layoutGroups)[N]
        )
        {
            std::uint8_t buf[VEST_PAYLOAD_SIZE];
            std::size_t copyLength = std::min(value.size(), sizeof(buf));
            std::memcpy(buf, value.c_str(), copyLength);

            applyVestGrouped(output, buf, layout, layoutGroups);
        }

      private:
        static const EffectData
          effectDataFromByte(const Effect effect, const uint8_t byte, const uint8_t maxValue = 100)
        {
            switch (effect) {
                case Effect::Vibro:
                    return VibroEffectData(::SenseShift::simpleMap<VibroEffectData::Intensity>(
                      byte,
                      maxValue,
                      VibroEffectData::INTENSITY_MAX
                    ));
                default:
                    throw std::runtime_error("Unknown effect");
            }
        }
    };
} // namespace SenseShift::BH
