#pragma once

#include <hand_interface.hpp>

#pragma region TaxtSuit X16

#define BH_LAYOUT_TACTSUITX40_SIZE_X 4
#define BH_LAYOUT_TACTSUITX40_SIZE_Y 5
#define BH_LAYOUT_TACTSUITX40_MAKE_POINT(x, y)                                  \
    SenseShift::Body::Haptics::PlaneMapper_Margin::mapPoint<oh_output_coord_t>( \
      x,                                                                        \
      y,                                                                        \
      (oh_output_coord_t) (BH_LAYOUT_TACTSUITX40_SIZE_X - 1),                   \
      (oh_output_coord_t) (BH_LAYOUT_TACTSUITX40_SIZE_Y - 1)                    \
    )

// X * Y for front and back
#define BH_LAYOUT_TACTSUITX40_SIZE 40
// clang-format off
#define BH_LAYOUT_TACTSUITX40 {                       \
    /* Front, left part */                            \
    /*  0 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 0) },  \
    /*  1 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 0) },  \
    /*  2 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 1) },  \
    /*  3 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 1) },  \
    /*  4 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 2) },  \
    /*  5 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 2) },  \
    /*  6 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 3) },  \
    /*  7 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 3) },  \
    /*  8 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 4) },  \
    /*  9 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 4) },  \
                                                      \
    /* Back */                                        \
    /* 11 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 0) },  \
    /* 11 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 0) },  \
    /* 12 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 1) },  \
    /* 13 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 1) },  \
    /* 14 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 2) },  \
    /* 15 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 2) },  \
    /* 16 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 3) },  \
    /* 17 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 3) },  \
    /* 18 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 4) },  \
    /* 19 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 4) },  \
                                                      \
    /* 20 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 0) },  \
    /* 21 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 0) },  \
    /* 22 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 1) },  \
    /* 23 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 1) },  \
    /* 24 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 2) },  \
    /* 25 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 2) },  \
    /* 26 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 3) },  \
    /* 27 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 3) },  \
    /* 28 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 4) },  \
    /* 29 */ { Target_t::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 4) },  \
                                                      \
    /* Front, again... Now right part */              \
    /* 30 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 0) },  \
    /* 31 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 0) },  \
    /* 32 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 1) },  \
    /* 33 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 1) },  \
    /* 34 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 2) },  \
    /* 35 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 2) },  \
    /* 36 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 3) },  \
    /* 37 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 3) },  \
    /* 38 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 4) },  \
    /* 39 */ { Target_t::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 4) }   \
}
// clang-format on

#pragma endregion BH_DEVICE_TACTSUITX40

namespace BH {
    using namespace SenseShift::Body::Hands::Haptics;

    typedef SenseShift::Body::Haptics::Effect_t Effect_t;
    typedef SenseShift::Body::Haptics::Target_t Target_t;
    typedef SenseShift::Body::Haptics::Position_t Position_t;
    typedef std::tuple<Target_t, Position_t> OutputLayout_t;

    // TactSuit X40 motor positions
    static constexpr const OutputLayout_t TactSuitX40Layout[] = BH_LAYOUT_TACTSUITX40;

    // TactGlove Wrist motor position
    static constexpr const Position_t WRIST_MOTOR_POSITION(127, 191);
    static constexpr const OutputLayout_t TactGloveLeftLayout[] = {
        { Target_t::HandLeftThumb, FINGERTIP_POSITION },  { Target_t::HandLeftIndex, FINGERTIP_POSITION },
        { Target_t::HandLeftMiddle, FINGERTIP_POSITION }, { Target_t::HandLeftRing, FINGERTIP_POSITION },
        { Target_t::HandLeftLittle, FINGERTIP_POSITION }, { Target_t::HandLeftDorsal, WRIST_MOTOR_POSITION },
    };
    static constexpr const OutputLayout_t TactGloveRightLayout[] = {
        { Target_t::HandRightThumb, FINGERTIP_POSITION },  { Target_t::HandRightIndex, FINGERTIP_POSITION },
        { Target_t::HandRightMiddle, FINGERTIP_POSITION }, { Target_t::HandRightRing, FINGERTIP_POSITION },
        { Target_t::HandRightLittle, FINGERTIP_POSITION }, { Target_t::HandRightDorsal, WRIST_MOTOR_POSITION },
    };
} // namespace BH
