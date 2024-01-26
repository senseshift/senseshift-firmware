#pragma once

namespace SenseShift {
    class IInitializable {
    public:
        virtual void init() = 0;
    };

    class ITickable {
    public:
        virtual void tick() = 0;
    };

    class Component : public IInitializable, public ITickable {
    public:
        /// @brief Initialize the component.
        ///
        /// Where the component's initialization should happen.
        /// Analogous to Arduino's setup(). This method is guaranteed to only be called once.
        ///
        /// Defaults to doing nothing.
        void init() override {}

        /// @brief Tick the component.
        ///
        /// This method will be called repeatedly.
        /// Analogous to Arduino's loop(). init() is guaranteed to be called before this.
        ///
        /// Defaults to doing nothing.
        void tick() override {}
    };
} // namespace SenseShift

