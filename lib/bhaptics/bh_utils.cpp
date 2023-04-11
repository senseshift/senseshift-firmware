#include "bh_utils.hpp"

#include <utility.hpp>

void BH::plainOutputTransformer(OH::Output* output, std::string& value, const oh_output_point_t* layout[], const size_t layoutSize, const oh_output_path_t path) {
  for (size_t i = 0; i < layoutSize; i++) {
    uint8_t byte = value[i];

    oh_output_data_t outputData{
      .point = *layout[i],
      .intensity = static_cast<oh_output_intensity_t>(map(byte, 0, 100, 0, OH_OUTPUT_INTENSITY_MAX)),
    };

    output->writeOutput(path, outputData);
  }
}

void BH::vestOutputTransformer(OH::Output* output, std::string& value, const oh_output_point_t* layout[], const size_t layoutSize) {
  for (size_t i = 0; i < layoutSize / 2; i++) {
    uint8_t byte = value[i];
    uint actIndex = i * 2;
    const auto path = (actIndex < 10 || actIndex >= 30) ? OUTPUT_PATH_CHEST_FRONT : OUTPUT_PATH_CHEST_BACK;

    const oh_output_data_t outputData0{
      .point = *layout[actIndex],
      .intensity = static_cast<oh_output_intensity_t>(map(((byte >> 4) & 0xf), 0, 15, 0, OH_OUTPUT_INTENSITY_MAX)),
    };

    const oh_output_data_t outputData1{
      .point = *layout[actIndex + 1],
      .intensity = static_cast<oh_output_intensity_t>(map((byte & 0xf), 0, 15, 0, OH_OUTPUT_INTENSITY_MAX)),
    };

    output->writeOutput(path, outputData0);
    output->writeOutput(path, outputData1);
  }
}

void BH::vestX16OutputTransformer(OH::Output* output, std::string& value, const oh_output_point_t* layout[], const size_t layoutSize, const uint8_t layoutGroups[], const size_t layoutGroupsSize) {
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

    const auto path = (i < 10 || i >= 30) ? OUTPUT_PATH_CHEST_FRONT : OUTPUT_PATH_CHEST_BACK;
    const oh_output_data_t outputData{
      .point = *layout[i],
      .intensity = static_cast<oh_output_intensity_t>(map(result[i], 0, 15, 0, OH_OUTPUT_INTENSITY_MAX)),
    };

    output->writeOutput(path, outputData);
  }
}
