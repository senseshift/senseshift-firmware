#pragma once

#include <cstdint>
#include <variant>

#include <abstract_actuator.hpp>
#include <point2.hpp>

#define OH_OUTPUT_COORD_T uint8_t
#define OH_OUTPUT_COORD_MAX UINT8_MAX

namespace SenseShift
{
    namespace Interface {
        namespace Body {
            namespace Haptics {
                typedef OH_OUTPUT_COORD_T Coordinate;
                typedef OH::Point2<Coordinate> Point;
                static const Coordinate CoordinateMin = 0;
                static const Coordinate CoordinateMax = OH_OUTPUT_COORD_MAX;

                typedef OH_OUTPUT_INTENSITY_T Intensity;
                static const Intensity IntensityMin = 0;
                static const Intensity IntensityMax = OH_OUTPUT_INTENSITY_MAX;

                typedef enum class Target {
                    ChestFront,
                    ChestBack,
                } Target;

                typedef enum class Layer {
                    Vibro,
                } Layer;

                typedef struct Message {
                    Target target;
                    Layer layer;
                    Point point;
                    Intensity intensity;
                } Message;
            } // namespace Haptics

            namespace ForceFeedback {
                typedef OH_OUTPUT_INTENSITY_T Intensity;
                static const Intensity IntensityMin = 0;
                static const Intensity IntensityMax = OH_OUTPUT_INTENSITY_MAX;

                /**
                 * Skeleton joints
                 *
                 * @see https://www.researchgate.net/figure/Proximal-Interphalangeal-PIP-Distal-Interphalangeal-DIP-and-Metacarpophalangeal_fig1_344683759
                 * @see https://www.researchgate.net/figure/Degrees-of-freedom-of-the-wrist-and-fingers-joints_fig1_264907843
                 */
                typedef enum class Joint {
                    Unknown,

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
                    Intensity intensity;
                } Message;
            } // namespace ForceFeedback
        } // namespace Body

        namespace Auxiliary {
            typedef OH_OUTPUT_INTENSITY_T Intensity;
            static const Intensity IntensityMin = 0;
            static const Intensity IntensityMax = OH_OUTPUT_INTENSITY_MAX;

            typedef enum class Target {
                PistolLeft,
                PistolRight,

                RifleStock,
                RifleBolt,
            } Target;

            typedef struct Message {
                Target target;
                Intensity intensity;
            } Message;
        } // namespace Auxiliary

        namespace Meta {
            typedef struct BatteryMessage {
                uint8_t percentage;
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
