#pragma once

#include "haptic_plane.hpp"

#include <utility.hpp>

#include <map>

namespace SenseShift {
    namespace Body {
        namespace Haptics {
            class HapticBody {
              public:
                typedef SenseShift::Interface::Body::Haptics::Target Target_t;
                typedef SenseShift::Interface::Body::Haptics::Layer Layer_t;
                typedef std::map<Target_t, Plane*> TargetMap_t;
                typedef std::map<Layer_t, TargetMap_t> LayerMap_t;

                HapticBody() = default;

                void setup()
                {
                    for (auto& [_, targets] : this->components) {
                        for (auto& [_, plane] : targets) {
                            plane->setup();
                        }
                    }
                };

                void addComponent(const Target_t, const Layer_t, Plane*);
                LayerMap_t* getComponents();

                void writeOutput(const Target_t, const Layer_t, const Plane::Point_t&, const Plane::Intensity_t);

                inline void handleMessage(const SenseShift::Interface::Body::Haptics::Message& message)
                {
                    this->writeOutput(message.target, message.layer, message.point, message.intensity);
                }

              private:
                LayerMap_t components{};

                inline void writeOutput_Unsafe(const Target_t target, const Layer_t layer, const Plane::Point_t& point, const Plane::Intensity_t intensity)
                {
                    (*this->components[layer][target]).writeOutput(point, intensity);
                }
            };
        } // namespace Haptics
    } // namespace Body
} // namespace SenseShift
