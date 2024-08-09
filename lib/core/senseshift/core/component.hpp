#pragma once

#define SS_INIT_NOT_NULL(ptr) \
    if ((ptr) != nullptr) {   \
        (ptr)->init();        \
    }
#define SS_TICK_NOT_NULL(ptr) \
    if ((ptr) != nullptr) {   \
        (ptr)->tick();        \
    }

namespace SenseShift {
class IInitializable {
  public:
    virtual ~IInitializable() = default;

    virtual void init() = 0;
};
} // namespace SenseShift
