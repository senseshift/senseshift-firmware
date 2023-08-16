#pragma once

#include <haptic_body.hpp>
#include <cstring>

namespace BH
{
    class Decoder
    {
      public:
        typedef SenseShift::Body::Haptics::VibroEffectData_t VibroEffectData_t;
        typedef SenseShift::Body::Haptics::EffectData_t EffectData_t;
        typedef SenseShift::Body::Haptics::Effect_t Effect_t;
        typedef SenseShift::Body::Haptics::Target_t Target_t;
        typedef SenseShift::Body::Haptics::Position_t Position_t;
        typedef std::tuple<Effect_t, Target_t, Position_t> OutputLayout_t;

        static const size_t VEST_LAYOUT_SIZE = 40;
        static const size_t VEST_PAYLOAD_SIZE = 20;

        /**
         * Apply plain-encoded data to the output.
         */
        template<size_t N>
        static void applyPlain(
            SenseShift::Body::Haptics::HapticBody* output,
            const uint8_t (&value)[N],
            const Position_t (&layout)[N],
            const Effect_t effect,
            const Target_t target
        ) {
            for (size_t i = 0; i < N; i++) {
                auto position = layout[i];
                uint8_t byte = value[i];

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
            SenseShift::Body::Haptics::HapticBody* output,
            std::string& value,
            const Position_t (&layout)[N],
            const Effect_t effect,
            const Target_t target
        ) {
            std::uint8_t buf[6];
            std::size_t copyLength = std::min(value.size(), sizeof(buf));
            std::memcpy(buf, value.c_str(), copyLength);

            applyPlain(output, buf, layout, effect, target);
        }

        /**
         * Apply vest-encoded data to the output.
         */
        static void applyVest(
            SenseShift::Body::Haptics::HapticBody* output,
            const uint8_t (&value)[VEST_PAYLOAD_SIZE],
            const Position_t (&layout)[VEST_LAYOUT_SIZE]
        ) {
            for (size_t i = 0; i < VEST_PAYLOAD_SIZE; i++) {
                uint8_t byte = value[i];
                uint actIndex = i * 2;
                const auto target = (actIndex < 10 || actIndex >= 30) ? Target_t::ChestFront : Target_t::ChestBack;

                output->effect({
                    .effect = Effect_t::Vibro,
                    .target = target,
                    .position = layout[actIndex],
                    .data = effectDataFromByte(
                        Effect_t::Vibro,
                        ((byte >> 4) & 0xf),
                        15
                    ),
                });
                output->effect({
                    .effect = Effect_t::Vibro,
                    .target = target,
                    .position = layout[actIndex + 1],
                    .data = effectDataFromByte(
                        Effect_t::Vibro,
                        (byte & 0xf),
                        15
                    ),
                });
            }
        }

        static void applyVest(
            SenseShift::Body::Haptics::HapticBody* output,
            std::string& value,
            const Position_t (&layout)[VEST_LAYOUT_SIZE]
        ) {
            std::uint8_t buf[VEST_PAYLOAD_SIZE];
            std::size_t copyLength = std::min(value.size(), sizeof(buf));
            std::memcpy(buf, value.c_str(), copyLength);

            applyVest(output, buf, layout);
        }

        /**
         * Apply grouped vest-encoded data to the output.
         */
        template <size_t N>
        static void applyVestGrouped(
            SenseShift::Body::Haptics::HapticBody* output,
            const uint8_t (&value)[VEST_PAYLOAD_SIZE],
            const Position_t (&layout)[VEST_LAYOUT_SIZE],
            const uint8_t (&layoutGroups)[N]
        ) {
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

                const auto target = (i < 10 || i >= 30) ? Target_t::ChestFront : Target_t::ChestBack;

                output->effect({
                    .effect = Effect_t::Vibro,
                    .target = target,
                    .position = layout[i],
                    .data = effectDataFromByte(
                        Effect_t::Vibro,
                        result[i],
                        15
                    ),
                });
            }
        }

        template <size_t N>
        static void applyVestGrouped(
            SenseShift::Body::Haptics::HapticBody* output,
            std::string& value,
            const Position_t (&layout)[VEST_LAYOUT_SIZE],
            const uint8_t (&layoutGroups)[N]
        ) {
            std::uint8_t buf[VEST_PAYLOAD_SIZE];
            std::size_t copyLength = std::min(value.size(), sizeof(buf));
            std::memcpy(buf, value.c_str(), copyLength);

            applyVestGrouped(output, buf, layout, layoutGroups);
        }

      private:
        static const EffectData_t effectDataFromByte(
            const Effect_t& effect,
            const uint8_t byte,
            const uint8_t maxValue = 100
        ) {
            switch (effect) {
                case Effect_t::Vibro:
                    return VibroEffectData_t(
                        OH::simpleMap<VibroEffectData_t::Intensity_t>(
                            byte, maxValue, VibroEffectData_t::INTENSITY_MAX
                        )
                    );
                default:
                    throw std::runtime_error("Unknown effect");
            }
        }
    };
} // namespace BH
