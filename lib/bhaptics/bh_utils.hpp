#pragma once

#include <haptic_body.hpp>
#include <string>

namespace BH {
    template<size_t N>
    void plainOutputTransformer(
      SenseShift::Body::Haptics::HapticBody* output,
      const char (&value)[N],
      const oh_output_point_t (&layout)[N],
      const SenseShift::Body::Haptics::Target_t target
    )
    {
        for (size_t i = 0; i < N; i++) {
            uint8_t byte = value[i];

            output->effect({
              .effect = SenseShift::Body::Haptics::Effect::Vibro,
              .target = target,
              .position = layout[i],
              .data = SenseShift::Body::Haptics::VibroEffect(
                OH::simpleMap<SenseShift::Body::Haptics::VibroEffect::Intensity_t>(
                  byte,
                  100,
                  SenseShift::Body::Haptics::VibroEffect::INTENSITY_MAX
                )
              ),
            });
        }
    }

    template<size_t N = 20>
    void vestOutputTransformer(
      SenseShift::Body::Haptics::HapticBody* output, const char (&value)[N], const oh_output_point_t (&layout)[N * 2]
    );
    void vestX16OutputTransformer(
      SenseShift::Body::Haptics::HapticBody* output,
      const std::string& value,
      const oh_output_point_t* layout,
      const size_t layoutSize,
      const uint8_t layoutGroups[],
      const size_t layoutGroupsSize
    );
} // namespace BH
