#include "bh_utils.hpp"

#include <utility.hpp>

namespace BH {
    template<size_t N = 20>
    void vestOutputTransformer(
      SenseShift::Body::Haptics::HapticBody* output, const char (&value)[N], const oh_output_point_t (&layout)[N * 2]
    )
    {
        for (size_t i = 0; i < N; i++) {
            uint8_t byte = value[i];
            uint actIndex = i * 2;
            const auto target = (actIndex < 10 || actIndex >= 30) ? SenseShift::Body::Haptics::Target::ChestFront
                                                                  : SenseShift::Body::Haptics::Target::ChestBack;

            output->effect({
              .effect = SenseShift::Body::Haptics::Effect::Vibro,
              .target = target,
              .position = layout[actIndex],
              .data = SenseShift::Body::Haptics::VibroEffect(
                OH::simpleMap<SenseShift::Body::Haptics::VibroEffect::Intensity_t>(
                  ((byte >> 4) & 0xf),
                  15,
                  SenseShift::Body::Haptics::VibroEffect::INTENSITY_MAX
                )
              ),
            });
            output->effect({
              .effect = SenseShift::Body::Haptics::Effect::Vibro,
              .target = target,
              .position = layout[actIndex + 1],
              .data = SenseShift::Body::Haptics::VibroEffect(
                OH::simpleMap<SenseShift::Body::Haptics::VibroEffect::Intensity_t>(
                  (byte & 0xf),
                  15,
                  SenseShift::Body::Haptics::VibroEffect::INTENSITY_MAX
                )
              ),
            });
        }
    }

    template void vestOutputTransformer<20>(
      SenseShift::Body::Haptics::HapticBody* output, const char (&value)[20], const oh_output_point_t (&layout)[40]
    );
} // namespace BH

void BH::vestX16OutputTransformer(
  SenseShift::Body::Haptics::HapticBody* output,
  const std::string& value,
  const oh_output_point_t* layout,
  const size_t layoutSize,
  const uint8_t layoutGroups[],
  const size_t layoutGroupsSize
)
{
    uint8_t result[layoutSize];

    // Unpack values
    for (auto i = 0; i < layoutSize / 2; i++) {
        uint8_t byte = value[i];
        uint actIndex = i * 2;

        result[actIndex] = (byte >> 4) & 0xf;
        result[actIndex + 1] = (byte & 0xf);
    }

    // Assign max value into each group
    for (auto i = 0; i < layoutGroupsSize; i++) {
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

    for (uint8_t i = 0; i < static_cast<uint8_t>(layoutSize); i++) {
        // take only meaningful values
        if (!OH::contains(layoutGroups, layoutSize, i)) {
            continue;
        }

        const auto target = (i < 10 || i >= 30) ? SenseShift::Body::Haptics::Target::ChestFront
                                                : SenseShift::Body::Haptics::Target::ChestBack;

        output->effect({
          .effect = SenseShift::Body::Haptics::Effect::Vibro,
          .target = target,
          .position = layout[i],
          .data = SenseShift::Body::Haptics::VibroEffect(
            static_cast<oh_output_intensity_t>(OH::accurateMap<long>(result[i], 0, 15, 0, OH_OUTPUT_INTENSITY_MAX))
          ),
        });
    }
}
