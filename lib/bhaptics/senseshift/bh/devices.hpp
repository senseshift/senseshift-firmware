#pragma once

#include <array>
#include <tuple>

#include "senseshift/body/hands/hands_interface.hpp"

#include <senseshift/body/haptics/body.hpp>
#include <senseshift/body/haptics/plane.hpp>

#pragma region BH_DEVICE_TACTSUITX40

#define BH_LAYOUT_TACTSUITX40_SIZE_X 4
#define BH_LAYOUT_TACTSUITX40_SIZE_Y 5
#define BH_LAYOUT_TACTSUITX40_MAKE_POINT(x, y)                                                          \
    ::SenseShift::Body::Haptics::PlaneMapper_Margin::mapPoint<::SenseShift::Body::Haptics::Coordinate>( \
      x,                                                                                                \
      y,                                                                                                \
      (::SenseShift::Body::Haptics::Coordinate)(BH_LAYOUT_TACTSUITX40_SIZE_X - 1),                      \
      (::SenseShift::Body::Haptics::Coordinate)(BH_LAYOUT_TACTSUITX40_SIZE_Y - 1)                       \
    )

// X * Y for front and back
#define BH_LAYOUT_TACTSUITX40_SIZE 40
// clang-format off
#define BH_LAYOUT_TACTSUITX40 {                                              \
    /* Front, left part */                                                   \
    /*  0 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 0) }, \
    /*  1 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 0) }, \
    /*  2 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 1) }, \
    /*  3 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 1) }, \
    /*  4 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 2) }, \
    /*  5 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 2) }, \
    /*  6 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 3) }, \
    /*  7 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 3) }, \
    /*  8 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 4) }, \
    /*  9 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 4) }, \
                                                                             \
    /* Back */                                                               \
    /* 11 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 0) },  \
    /* 11 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 0) },  \
    /* 12 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 1) },  \
    /* 13 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 1) },  \
    /* 14 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 2) },  \
    /* 15 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 2) },  \
    /* 16 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 3) },  \
    /* 17 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 3) },  \
    /* 18 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(0, 4) },  \
    /* 19 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(1, 4) },  \
                                                                             \
    /* 20 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 0) },  \
    /* 21 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 0) },  \
    /* 22 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 1) },  \
    /* 23 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 1) },  \
    /* 24 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 2) },  \
    /* 25 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 2) },  \
    /* 26 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 3) },  \
    /* 27 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 3) },  \
    /* 28 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 4) },  \
    /* 29 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 4) },  \
                                                                             \
    /* Front, again... Now right part */                                     \
    /* 30 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 0) }, \
    /* 31 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 0) }, \
    /* 32 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 1) }, \
    /* 33 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 1) }, \
    /* 34 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 2) }, \
    /* 35 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 2) }, \
    /* 36 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 3) }, \
    /* 37 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 3) }, \
    /* 38 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(2, 4) }, \
    /* 39 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX40_MAKE_POINT(3, 4) }  \
}
// clang-format on

#pragma endregion BH_DEVICE_TACTSUITX40

#pragma region BH_DEVICE_TACTSUITX16

#define BH_LAYOUT_TACTSUITX16_SIZE_X 4
#define BH_LAYOUT_TACTSUITX16_SIZE_Y 2
#define BH_LAYOUT_TACTSUITX16_MAKE_POINT(x, y)                                                          \
    ::SenseShift::Body::Haptics::PlaneMapper_Margin::mapPoint<::SenseShift::Body::Haptics::Coordinate>( \
      x,                                                                                                \
      y,                                                                                                \
      (::SenseShift::Body::Haptics::Coordinate)(BH_LAYOUT_TACTSUITX16_SIZE_X - 1),                      \
      (::SenseShift::Body::Haptics::Coordinate)(BH_LAYOUT_TACTSUITX16_SIZE_Y - 1)                       \
    )

// X16 suit uses the same packets structure as x40 suit and performs motor grouping in firmware
#define BH_LAYOUT_TACTSUITX16_SIZE 40
// clang-format off
#define BH_LAYOUT_TACTSUITX16 {                                                       \
    /* Front, left part */                                                            \
    /*  0 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 0) }, /*  0 */ \
    /*  1 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 0) }, /*  1 */ \
    /*  2 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 0) }, /*  4 */ \
    /*  3 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 0) }, /*  5 */ \
                                                                                      \
    /*  4 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 1) }, /*  8 */ \
    /*  5 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 1) }, /*  9 */ \
    /*  6 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 1) }, /* 12 */ \
    /*  7 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 1) }, /* 13 */ \
    /*  8 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 1) }, /* 16 */ \
    /*  9 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 1) }, /* 17 */ \
                                                                                      \
    /* Back */                                                                        \
    /* 10 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 0) }, /*  0 */  \
    /* 11 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 0) }, /*  1 */  \
    /* 12 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 0) }, /*  4 */  \
    /* 13 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 0) }, /*  5 */  \
                                                                                      \
    /* 14 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 1) }, /*  8 */  \
    /* 15 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 1) }, /*  9 */  \
    /* 16 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 1) }, /* 12 */  \
    /* 17 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 1) }, /* 13 */  \
    /* 18 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(0, 1) }, /* 16 */  \
    /* 19 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(1, 1) }, /* 17 */  \
                                                                                      \
    /* 20 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 0) }, /*  2 */  \
    /* 21 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 0) }, /*  3 */  \
    /* 22 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 0) }, /*  4 */  \
    /* 23 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 0) }, /*  7 */  \
                                                                                      \
    /* 24 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 1) }, /* 10 */  \
    /* 25 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 1) }, /* 11 */  \
    /* 26 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 1) }, /* 14 */  \
    /* 27 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 1) }, /* 15 */  \
    /* 28 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 1) }, /* 18 */  \
    /* 29 */ { Target::ChestBack, BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 1) }, /* 19 */  \
                                                                                      \
    /* Front, again... Now right part */                                              \
    /* 30 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 0) }, /*  2 */ \
    /* 31 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 0) }, /*  3 */ \
    /* 32 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 0) }, /*  4 */ \
    /* 33 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 0) }, /*  7 */ \
                                                                                      \
    /* 34 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 1) }, /* 10 */ \
    /* 35 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 1) }, /* 11 */ \
    /* 36 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 1) }, /* 14 */ \
    /* 37 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 1) }, /* 15 */ \
    /* 38 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(2, 1) }, /* 18 */ \
    /* 39 */ { Target::ChestFront, BH_LAYOUT_TACTSUITX16_MAKE_POINT(3, 1) }, /* 19 */ \
}
// clang-format on

// Ouput indices, responsible for x40 => x16 grouping
#define BH_LAYOUT_TACTSUITX16_GROUPS                               \
    {                                                              \
        0, 1, 4, 5, 10, 11, 14, 15, 20, 21, 24, 25, 30, 31, 34, 35 \
    }
#define BH_LAYOUT_TACTSUITX16_GROUPS_SIZE 16

#pragma endregion BH_DEVICE_TACTSUITX16

#pragma region BH_DEVICE_TACTAL

#define BH_LAYOUT_TACTAL_SIZE_X 6
#define BH_LAYOUT_TACTAL_SIZE_Y 1
#define BH_LAYOUT_TACTAL_MAKE_POINT(x, y)                                                               \
    ::SenseShift::Body::Haptics::PlaneMapper_Margin::mapPoint<::SenseShift::Body::Haptics::Coordinate>( \
      x,                                                                                                \
      y,                                                                                                \
      (::SenseShift::Body::Haptics::Coordinate)(BH_LAYOUT_TACTAL_SIZE_X - 1),                           \
      (::SenseShift::Body::Haptics::Coordinate)(BH_LAYOUT_TACTAL_SIZE_Y - 1)                            \
    )

#define BH_LAYOUT_TACTAL_SIZE (BH_LAYOUT_TACTAL_SIZE_X * BH_LAYOUT_TACTAL_SIZE_Y)
// clang-format off
#define BH_LAYOUT_TACTAL {           \
  BH_LAYOUT_TACTAL_MAKE_POINT(0, 0), \
  BH_LAYOUT_TACTAL_MAKE_POINT(1, 0), \
  BH_LAYOUT_TACTAL_MAKE_POINT(2, 0), \
  BH_LAYOUT_TACTAL_MAKE_POINT(3, 0), \
  BH_LAYOUT_TACTAL_MAKE_POINT(4, 0), \
  BH_LAYOUT_TACTAL_MAKE_POINT(5, 0), \
}
// clang-format on

#pragma endregion BH_DEVICE_TACTVISOR

#pragma region BH_DEVICE_TACTAL

#define BH_LAYOUT_TACTVISOR_SIZE_X 4
#define BH_LAYOUT_TACTVISOR_SIZE_Y 1
#define BH_LAYOUT_TACTVISOR_MAKE_POINT(x, y)                                                            \
    ::SenseShift::Body::Haptics::PlaneMapper_Margin::mapPoint<::SenseShift::Body::Haptics::Coordinate>( \
      x,                                                                                                \
      y,                                                                                                \
      (::SenseShift::Body::Haptics::Coordinate)(BH_LAYOUT_TACTVISOR_SIZE_X - 1),                        \
      (::SenseShift::Body::Haptics::Coordinate)(BH_LAYOUT_TACTVISOR_SIZE_Y - 1)                         \
    )

#define BH_LAYOUT_TACTVISOR_SIZE (BH_LAYOUT_TACTVISOR_SIZE_X * BH_LAYOUT_TACTVISOR_SIZE_Y)
// clang-format off
#define BH_LAYOUT_TACTVISOR {           \
  BH_LAYOUT_TACTVISOR_MAKE_POINT(0, 0), \
  BH_LAYOUT_TACTVISOR_MAKE_POINT(1, 0), \
  BH_LAYOUT_TACTVISOR_MAKE_POINT(2, 0), \
  BH_LAYOUT_TACTVISOR_MAKE_POINT(3, 0), \
}
// clang-format on

#pragma endregion BH_DEVICE_TACTVISOR

#pragma region BH_DEVICE_TACTOSY2

#define BH_LAYOUT_TACTOSY2_SIZE_X 3
#define BH_LAYOUT_TACTOSY2_SIZE_Y 2
#define BH_LAYOUT_TACTOSY2_MAKE_POINT(x, y)                                                             \
    ::SenseShift::Body::Haptics::PlaneMapper_Margin::mapPoint<::SenseShift::Body::Haptics::Coordinate>( \
      x,                                                                                                \
      y,                                                                                                \
      (::SenseShift::Body::Haptics::Coordinate)(BH_LAYOUT_TACTOSY2_SIZE_X - 1),                         \
      (::SenseShift::Body::Haptics::Coordinate)(BH_LAYOUT_TACTOSY2_SIZE_Y - 1)                          \
    )

#define BH_LAYOUT_TACTOSY2_SIZE (BH_LAYOUT_TACTOSY2_SIZE_X * BH_LAYOUT_TACTOSY2_SIZE_Y)
// clang-format off
#define BH_LAYOUT_TACTOSY2 {                                                                                       \
    BH_LAYOUT_TACTOSY2_MAKE_POINT(0, 0), BH_LAYOUT_TACTOSY2_MAKE_POINT(1, 0), BH_LAYOUT_TACTOSY2_MAKE_POINT(2, 0), \
    BH_LAYOUT_TACTOSY2_MAKE_POINT(0, 1), BH_LAYOUT_TACTOSY2_MAKE_POINT(1, 1), BH_LAYOUT_TACTOSY2_MAKE_POINT(2, 1), \
}
// clang-format on

#pragma endregion BH_DEVICE_TACTOSY2

#pragma region BH_DEVICE_TACTOSYH

#define BH_LAYOUT_TACTOSYH_SIZE_X 1
#define BH_LAYOUT_TACTOSYH_SIZE_Y 3
#define BH_LAYOUT_TACTOSYH_MAKE_POINT(x, y)                                                             \
    ::SenseShift::Body::Haptics::PlaneMapper_Margin::mapPoint<::SenseShift::Body::Haptics::Coordinate>( \
      x,                                                                                                \
      y,                                                                                                \
      (::SenseShift::Body::Haptics::Coordinate)(BH_LAYOUT_TACTOSYH_SIZE_X - 1),                         \
      (::SenseShift::Body::Haptics::Coordinate)(BH_LAYOUT_TACTOSYH_SIZE_Y - 1)                          \
    )

#define BH_LAYOUT_TACTOSYH_SIZE (BH_LAYOUT_TACTOSYH_SIZE_X * BH_LAYOUT_TACTOSYH_SIZE_Y)
// clang-format off
#define BH_LAYOUT_TACTOSYH { BH_LAYOUT_TACTOSYH_MAKE_POINT(0, 0), BH_LAYOUT_TACTOSYH_MAKE_POINT(0, 1), BH_LAYOUT_TACTOSYH_MAKE_POINT(0, 2) }
// clang-format on

#pragma endregion BH_DEVICE_TACTOSYH

#pragma region BH_DEVICE_TACTOSYF

#define BH_LAYOUT_TACTOSYF_SIZE_X 1
#define BH_LAYOUT_TACTOSYF_SIZE_Y 3
#define BH_LAYOUT_TACTOSYF_MAKE_POINT(x, y)                                                             \
    ::SenseShift::Body::Haptics::PlaneMapper_Margin::mapPoint<::SenseShift::Body::Haptics::Coordinate>( \
      x,                                                                                                \
      y,                                                                                                \
      (::SenseShift::Body::Haptics::Coordinate)(BH_LAYOUT_TACTOSYF_SIZE_X - 1),                         \
      (::SenseShift::Body::Haptics::Coordinate)(BH_LAYOUT_TACTOSYF_SIZE_Y - 1)                          \
    )

#define BH_LAYOUT_TACTOSYF_SIZE (BH_LAYOUT_TACTOSYF_SIZE_X * BH_LAYOUT_TACTOSYF_SIZE_Y)
// clang-format off
#define BH_LAYOUT_TACTOSYF { BH_LAYOUT_TACTOSYF_MAKE_POINT(0, 0), BH_LAYOUT_TACTOSYF_MAKE_POINT(0, 1), BH_LAYOUT_TACTOSYF_MAKE_POINT(0, 2), }
// clang-format on

#pragma endregion BH_DEVICE_TACTOSYF

#pragma region BH_DEVIDE_TACTGLOVE

// clang-format off
#define BH_LAYOUT_TACTGLOVE_SIZE 6
// TactGlove (Left) motor positions
#define BH_LAYOUT_TACTGLOVE_LEFT {                      \
    { Target::HandLeftThumb,  FINGERTIP_POSITION },   \
    { Target::HandLeftIndex,  FINGERTIP_POSITION },   \
    { Target::HandLeftMiddle, FINGERTIP_POSITION },   \
    { Target::HandLeftRing,   FINGERTIP_POSITION },   \
    { Target::HandLeftLittle, FINGERTIP_POSITION },   \
    { Target::HandLeftDorsal, WRIST_MOTOR_POSITION }, \
}
// TactGlove (Right) motor positions
#define BH_LAYOUT_TACTGLOVE_RIGHT {                     \
    { Target::HandRightThumb,  FINGERTIP_POSITION },  \
    { Target::HandRightIndex,  FINGERTIP_POSITION },  \
    { Target::HandRightMiddle, FINGERTIP_POSITION },  \
    { Target::HandRightRing,   FINGERTIP_POSITION },  \
    { Target::HandRightLittle, FINGERTIP_POSITION },  \
    { Target::HandRightDorsal, WRIST_MOTOR_POSITION } \
}
// clang-format on

#pragma endregion BH_DEVIDE_TACTGLOVE

namespace SenseShift::BH {
using namespace ::SenseShift::Body::Hands::Haptics;
using namespace ::SenseShift::Body::Haptics;

using HandSide = ::SenseShift::Body::Hands::HandSide;

using OutputLayout = std::tuple<::SenseShift::Body::Haptics::Target, SenseShift::Body::Haptics::Position>;

// TactGlove Wrist motor position
static constexpr const Position WRIST_MOTOR_POSITION(127, 191);
static constexpr const std::array<OutputLayout, BH_LAYOUT_TACTGLOVE_SIZE> TactGloveLeftLayout = {
    BH_LAYOUT_TACTGLOVE_LEFT
};
static constexpr const std::array<OutputLayout, BH_LAYOUT_TACTGLOVE_SIZE> TactGloveRightLayout = {
    BH_LAYOUT_TACTGLOVE_RIGHT
};

inline void addTactGloveActuators(
  FloatBody* hapticBody,
  const HandSide side,
  FloatBody::Plane::Actuator* const thumb,
  FloatBody::Plane::Actuator* const index,
  FloatBody::Plane::Actuator* const middle,
  FloatBody::Plane::Actuator* const ring,
  FloatBody::Plane::Actuator* const little,
  FloatBody::Plane::Actuator* const wrist
)
{
    const auto& layout = (side == HandSide::Left) ? TactGloveLeftLayout : TactGloveRightLayout;

    if (thumb != nullptr) {
        hapticBody->addTarget(std::get<0>(layout[0]), new FloatPlane({ { std::get<1>(layout[0]), thumb } }));
    }

    if (index != nullptr) {
        hapticBody->addTarget(std::get<0>(layout[1]), new FloatPlane({ { std::get<1>(layout[1]), index } }));
    }

    if (middle != nullptr) {
        hapticBody->addTarget(std::get<0>(layout[2]), new FloatPlane({ { std::get<1>(layout[2]), middle } }));
    }

    if (ring != nullptr) {
        hapticBody->addTarget(std::get<0>(layout[3]), new FloatPlane({ { std::get<1>(layout[3]), ring } }));
    }

    if (little != nullptr) {
        hapticBody->addTarget(std::get<0>(layout[4]), new FloatPlane({ { std::get<1>(layout[4]), little } }));
    }

    if (wrist != nullptr) {
        hapticBody->addTarget(std::get<0>(layout[5]), new FloatPlane({ { std::get<1>(layout[5]), wrist } }));
    }
}
} // namespace SenseShift::BH
