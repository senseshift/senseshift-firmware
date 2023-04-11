#pragma once

#include <string>
#include <output.hpp>

namespace BH {
  void plainOutputTransformer(OH::Output* output, std::string& value, const oh_output_point_t* layout[], const size_t layoutSize, const oh_output_path_t path);
  void vestOutputTransformer(OH::Output* output, std::string& value, const oh_output_point_t* layout[], const size_t layoutSize);
  void vestX16OutputTransformer(OH::Output* output, std::string& value, const oh_output_point_t* layout[], const size_t layoutSize, const uint8_t layoutGroups[], const size_t layoutGroupsSize);
} // namespace BH
