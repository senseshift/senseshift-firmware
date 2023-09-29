#pragma once

#include "senseshift/body/haptics/interface.hpp"
#include "senseshift/body/haptics/plane.hpp"

#include <senseshift/utility.hpp>

#include <map>

namespace SenseShift::Body::Haptics {
    class HapticBody {
      public:
        using AuctiativePlane = std::variant<VibroPlane*>;
        using PlaneTargetMap = std::multimap<Target, AuctiativePlane>;
        using VibroTargetMap = std::map<Target, VibroPlane*>;

        HapticBody(){};

        void setup();

        void effect(const EffectRequest&);

        void addTarget(const Target, VibroPlane* plane);

        [[nodiscard]] const PlaneTargetMap* getTargets() const { return &allTargets; }

      private:
        PlaneTargetMap allTargets{};
        VibroTargetMap vibroTargets{};
    };
} // namespace SenseShift::Body::Haptics
