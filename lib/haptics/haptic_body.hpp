#pragma once

#include "haptic_plane.hpp"
#include "haptics_interface.hpp"

#include <types.hpp>
#include <utility.hpp>

#include <map>

namespace SenseShift::Body::Haptics {
    class HapticBody {
      public:
        typedef std::map<Target_t, VibroPlane*> VibroTargetMap_t;

        HapticBody(){};

        void setup();

        void effect(const EffectRequest_t&);

        void addTarget(const Target_t, VibroPlane* plane);

      private:
        VibroTargetMap_t vibroTargets{};
    };
} // namespace SenseShift::Body::Haptics
