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

class ITickable {
  public:
    virtual ~ITickable() = default;

    virtual void tick() = 0;
};

class Component : public virtual IInitializable, public virtual ITickable {
  public:
    /// @brief Initialize the component.
    ///
    /// Where the component's initialization should happen.
    /// Analogous to Arduino's setup(). This method is guaranteed to only be called once.
    ///
    /// Defaults to doing nothing.
    void init() override
    {
    }

    /// @brief Tick the component.
    ///
    /// This method will be called repeatedly.
    /// Analogous to Arduino's loop(). init() is guaranteed to be called before this.
    ///
    /// Defaults to doing nothing.
    void tick() override
    {
    }
};
} // namespace SenseShift
