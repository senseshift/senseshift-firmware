#pragma once

#include <cstring>
#include <haptic_body.hpp>

namespace SenseShift::BH {
    class Decoder {
      public:
        using VibroEffectData_t = ::SenseShift::Body::Haptics::VibroEffectData_t;
        using EffectData_t = ::SenseShift::Body::Haptics::EffectData_t;

        using Effect_t = ::SenseShift::Body::Haptics::Effect_t;
        using Target_t = ::SenseShift::Body::Haptics::Target_t;
        using Position_t = ::SenseShift::Body::Haptics::Position_t;

        using HapticBody_t = ::SenseShift::Body::Haptics::HapticBody;

        typedef std::tuple<Target_t, Position_t> OutputLayout_t;

        static const size_t VEST_LAYOUT_SIZE = 40;
        static const size_t VEST_PAYLOAD_SIZE = 20;

        template<size_t N>
        static void applyPlain(
          HapticBody_t* output, const uint8_t (&value)[N], const OutputLayout_t (&layout)[N], const Effect_t effect
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
          applyPlain(HapticBody_t* output, std::string& value, const OutputLayout_t (&layout)[N], const Effect_t effect)
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
          HapticBody_t* output,
          const uint8_t (&value)[N],
          const Position_t (&layout)[N],
          const Effect_t effect,
          const Target_t target
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
          HapticBody_t* output,
          std::string& value,
          const Position_t (&layout)[N],
          const Effect_t effect,
          const Target_t target
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
          HapticBody_t* output,
          const uint8_t (&value)[VEST_PAYLOAD_SIZE],
          const OutputLayout_t (&layout)[VEST_LAYOUT_SIZE]
        )
        {
            for (size_t i = 0; i < VEST_PAYLOAD_SIZE; i++) {
                uint8_t byte = value[i];
                uint actIndex = i * 2;
                output->effect({
                  .effect = Effect_t::Vibro,
                  .target = std::get<0>(layout[actIndex]),
                  .position = std::get<1>(layout[actIndex]),
                  .data = effectDataFromByte(Effect_t::Vibro, ((byte >> 4) & 0xf), 15),
                });
                output->effect({
                  .effect = Effect_t::Vibro,
                  .target = std::get<0>(layout[actIndex + 1]),
                  .position = std::get<1>(layout[actIndex + 1]),
                  .data = effectDataFromByte(Effect_t::Vibro, (byte & 0xf), 15),
                });
            }
        }

        static void
          applyVest(HapticBody_t* output, std::string& value, const OutputLayout_t (&layout)[VEST_LAYOUT_SIZE])
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
          HapticBody_t* output,
          const uint8_t (&value)[VEST_PAYLOAD_SIZE],
          const OutputLayout_t (&layout)[VEST_LAYOUT_SIZE],
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
                if (!OH::contains(layoutGroups, VEST_LAYOUT_SIZE, i)) {
                    continue;
                }

                const auto target = std::get<0>(layout[i]);
                const auto position = std::get<1>(layout[i]);

                output->effect({
                  .effect = Effect_t::Vibro,
                  .target = target,
                  .position = position,
                  .data = effectDataFromByte(Effect_t::Vibro, result[i], 15),
                });
            }
        }

        template<size_t N>
        static void applyVestGrouped(
          HapticBody_t* output,
          std::string& value,
          const OutputLayout_t (&layout)[VEST_LAYOUT_SIZE],
          const uint8_t (&layoutGroups)[N]
        )
        {
            std::uint8_t buf[VEST_PAYLOAD_SIZE];
            std::size_t copyLength = std::min(value.size(), sizeof(buf));
            std::memcpy(buf, value.c_str(), copyLength);

            applyVestGrouped(output, buf, layout, layoutGroups);
        }

      private:
        static const EffectData_t
          effectDataFromByte(const Effect_t effect, const uint8_t byte, const uint8_t maxValue = 100)
        {
            switch (effect) {
                case Effect_t::Vibro:
                    return VibroEffectData_t(
                      OH::simpleMap<VibroEffectData_t::Intensity_t>(byte, maxValue, VibroEffectData_t::INTENSITY_MAX)
                    );
                default:
                    throw std::runtime_error("Unknown effect");
            }
        }
    };
} // namespace SenseShift::BH
