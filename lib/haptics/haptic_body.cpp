#include "haptic_body.hpp"

#include <logging.hpp>

namespace SenseShift {
    namespace Body {
        namespace Haptics {
            void HapticBody::addComponent(const Target_t target, const Layer_t layer, Plane* plane)
            {
                auto layerSearch = this->components.find(layer);

                if (layerSearch == this->components.end()) {
                    // if layer does not exist, create it
                    this->components.insert(std::make_pair(layer, TargetMap_t{}));
                    layerSearch = this->components.find(layer);
                }

                auto targetSearch = (*layerSearch).second.find(target);

                if (targetSearch == (*layerSearch).second.end()) {
                    // if target does not exist, create it
                    (*layerSearch).second.insert(std::make_pair(target, nullptr));
                    targetSearch = (*layerSearch).second.find(target);
                }

                (*targetSearch).second = plane;
            }

            HapticBody::LayerMap_t* HapticBody::getComponents()
            {
                return &this->components;
            }

            void HapticBody::writeOutput(const Target_t target, const Layer_t layer, const Plane::Point_t& point, const Plane::Intensity_t intensity)
            {
                auto layerSearch = this->components.find(layer);
                if (layerSearch == this->components.end()) {
                    // if layer does not exist, skip
                    log_w("No layer found for message %d", layer);
                    return;
                }

                auto targetSearch = (*layerSearch).second.find(target);
                if (targetSearch == (*layerSearch).second.end()) {
                    // if target does not exist, skip
                    log_w("No target found for message %d (layer %d)", target, layer);
                    return;
                }

                this->writeOutput_Unsafe(target, layer, point, intensity);
            }
        }
    }
};
