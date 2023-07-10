#pragma once

#include "haptic_constants.h"
#include "haptic_plane.hpp"

#include <types.hpp>
#include <utility.hpp>

#include <map>

namespace OH {
    class HapticBody {
      private:
        typedef std::map<oh_output_path_t, HapticPlane*> oh_output_components_map_t;
        std::map<oh_output_path_t, HapticPlane*> components{};

      public:
        HapticBody(){};

        void addComponent(const oh_output_path_t, HapticPlane*);
        oh_output_components_map_t* getComponents();

        void writeOutput(const oh_output_path_t, const oh_output_data_t&);

        void setup()
        {
            for (auto& component : this->components) {
                component.second->setup();
            }
        };
    };
} // namespace OH
