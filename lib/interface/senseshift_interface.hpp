#pragma once

#include "senseshift_types.hpp"

#include <variant>

#include <point2.hpp>

namespace SenseShift
{
    namespace Interface {
        namespace Body {
            namespace Haptics {
                typedef OH_OUTPUT_COORD_T Coordinate_t;
                typedef OH::Point2<Coordinate_t> Point_t;
                static const Coordinate_t COORDINATE_MIN = 0;
                static const Coordinate_t COORDINATE_MAX = OH_OUTPUT_COORD_MAX;

                typedef OH_OUTPUT_INTENSITY_T Intensity_t;
                static const Intensity_t INTENSITY_MIN = 0;
                static const Intensity_t INTENSITY_MAX = OH_OUTPUT_INTENSITY_MAX;

                typedef int32_t TargetIndex_t;
                static const TargetIndex_t TARGET_INVALID = -1;
                typedef enum class Target : TargetIndex_t {
                    Invalid = TARGET_INVALID,
                    ChestFront = 0,
                    ChestBack = 1,
                } Target;

                typedef int8_t LayerIndex_t;
                static const LayerIndex_t LAYER_INVALID = -1;
                typedef enum class Layer : LayerIndex_t {
                    Invalid = LAYER_INVALID,
                    Vibro = 0,
                } Layer;

                /**
                 * Spatial haptics message
                 */
                typedef struct Message {
                    Target target;
                    Layer layer;
                    Point_t point;
                    Intensity_t intensity;
                } Message;
            } // namespace Haptics

            namespace ForceFeedback {
                typedef OH_OUTPUT_INTENSITY_T Intensity_t;
                static const Intensity_t INTENSITY_MIN = 0;
                static const Intensity_t INTENSITY_MAX = OH_OUTPUT_INTENSITY_MAX;

                typedef int32_t JointIndex_t;
                static const JointIndex_t JOINT_INVALID = -1;

                /**
                 * Skeleton joints
                 *
                 * @see https://www.researchgate.net/figure/Proximal-Interphalangeal-PIP-Distal-Interphalangeal-DIP-and-Metacarpophalangeal_fig1_344683759
                 * @see https://www.researchgate.net/figure/Degrees-of-freedom-of-the-wrist-and-fingers-joints_fig1_264907843
                 */
                typedef enum class Joint : JointIndex_t {
                    Invalid = JOINT_INVALID,

                    #pragma region Left Hand
                    HandLeftThumbCMC_Yaw,
                    HandLeftThumbCMC_Pitch,
                    HandLeftThumbMCP_Yaw,
                    HandLeftThumbMCP_Pitch,
                    HandLeftThumbIP_Pitch,

                    HandLeftIndexMCP_Yaw,
                    HandLeftIndexMCP_Pitch,
                    HandLeftIndexPIP_Pitch,
                    HandLeftIndexDIP_Pitch,

                    HandLeftMiddleMCP_Yaw,
                    HandLeftMiddleMCP_Pitch,
                    HandLeftMiddlePIP_Pitch,
                    HandLeftMiddleDIP_Pitch,

                    HandLeftRingMCP_Yaw,
                    HandLeftRingMCP_Pitch,
                    HandLeftRingPIP_Pitch,
                    HandLeftRingDIP_Pitch,

                    HandLeftLittleMCP_Yaw,
                    HandLeftLittleMCP_Pitch,
                    HandLeftLittlePIP_Pitch,
                    HandLeftLittleDIP_Pitch,

                    HandLeftWristYaw,
                    HandLeftWristPitch,
                    HandLeftWristRoll,
                    #pragma endregion

                    #pragma region Right Hand
                    HandRightThumbCMC_Yaw,
                    HandRightThumbCMC_Pitch,
                    HandRightThumbMCP_Yaw,
                    HandRightThumbMCP_Pitch,
                    HandRightThumbIP_Pitch,

                    HandRightIndexMCP_Yaw,
                    HandRightIndexMCP_Pitch,
                    HandRightIndexPIP_Pitch,
                    HandRightIndexDIP_Pitch,

                    HandRightMiddleMCP_Yaw,
                    HandRightMiddleMCP_Pitch,
                    HandRightMiddlePIP_Pitch,
                    HandRightMiddleDIP_Pitch,

                    HandRightRingMCP_Yaw,
                    HandRightRingMCP_Pitch,
                    HandRightRingPIP_Pitch,
                    HandRightRingDIP_Pitch,

                    HandRightLittleMCP_Yaw,
                    HandRightLittleMCP_Pitch,
                    HandRightLittlePIP_Pitch,
                    HandRightLittleDIP_Pitch,

                    HandRightWristYaw,
                    HandRightWristPitch,
                    HandRightWristRoll,
                    #pragma endregion
                } Joint;

                typedef struct Message {
                    Joint joint;
                    Intensity_t intensity;
                } Message;
            } // namespace ForceFeedback
        } // namespace Body

        namespace Auxiliary {
            typedef OH_OUTPUT_INTENSITY_T Intensity_t;
            static const Intensity_t INTENSITY_MIN = 0;
            static const Intensity_t INTENSITY_MAX = OH_OUTPUT_INTENSITY_MAX;

            typedef int32_t TargetIndex_t;
            static const TargetIndex_t TARGET_INVALID = -1;
            typedef enum class Target : TargetIndex_t {
                Invalid = TARGET_INVALID,
                PistolLeft,
                PistolRight,

                RifleStock,
                RifleBolt,
            } Target;

            typedef struct Message {
                Target target;
                Intensity_t intensity;
            } Message;
        } // namespace Auxiliary

        namespace Meta {
            typedef struct BatteryMessage {
                typedef uint8_t Percentage_t;
                static const Percentage_t PERCENTAGE_MIN = 0;
                static const Percentage_t PERCENTAGE_MAX = 255;

                typedef int8_t StatusIndex_t;
                static const StatusIndex_t STATUS_INVALID = -1;
                typedef enum class Status : StatusIndex_t {
                    Invalid = STATUS_INVALID,
                    Unknown = 0,
                    Discharging,
                    Charging,
                    Full,
                } Status;

                Percentage_t percentage;
                Status status;
            } BatteryMessage;

            typedef std::variant<
                BatteryMessage
            > Message;
        } // namespace Meta

        typedef std::variant<
            Body::Haptics::Message,
            Body::ForceFeedback::Message,
            Auxiliary::Message,
            Meta::Message
        > Message;
    } // namespace Interface
} // namespace SenseShift
