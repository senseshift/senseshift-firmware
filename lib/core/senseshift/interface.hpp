#pragma once

namespace SenseShift {
    struct IInitializable {
        virtual void init() = 0;
    };

    struct ITickable {
        virtual void tick() = 0;
    };
} // namespace SenseShift
