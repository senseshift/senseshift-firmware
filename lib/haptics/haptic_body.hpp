#pragma once

#include "haptic_plane.hpp"
#include "haptics_interface.hpp"

#include <types.hpp>
#include <utility.hpp>

#include <map>

namespace SenseShift::Body::Haptics {
    class HapticBody {
      public:
        typedef std::variant<VibroPlane*> AuctiativePlane_t;
        typedef std::multimap<Target_t, AuctiativePlane_t> PlaneTargetMap_t;
        typedef std::map<Target_t, VibroPlane*> VibroTargetMap_t;

        HapticBody(){};

        void setup();

        void effect(const EffectRequest_t&);

        void addTarget(const Target_t, VibroPlane* plane);

        [[nodiscard]] const PlaneTargetMap_t* getTargets() const
        {
            return &allTargets;
        }

      private:
        PlaneTargetMap_t allTargets{};
        VibroTargetMap_t vibroTargets{};
    };
} // namespace SenseShift::Body::Haptics
