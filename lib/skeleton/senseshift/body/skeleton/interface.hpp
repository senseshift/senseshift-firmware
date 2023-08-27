#pragma once

#include <cstdint>
#include <map>

namespace SenseShift::Body::Skeleton {
    typedef std::uint16_t JointIndex_t;
    static const JointIndex_t JOINT_INVALID = 0xFFFF;
    typedef enum class Joint : JointIndex_t {
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

        Invalid = JOINT_INVALID,
    } Joint_t;

    typedef std::uint16_t JointValue_t;
    static const JointValue_t JOINT_VALUE_MIN = 0;
    static const JointValue_t JOINT_VALUE_MAX = 4095;

    typedef std::pair<Joint, JointValue_t> JointState_t;
    typedef std::map<Joint, JointValue_t> JointStateMap_t;

    // typedef std::uint16_t ComplexJointIndex_t;
    // static const ComplexJointIndex_t COMPLEX_JOINT_INVALID = 0xFFFF;
    // typedef enum class ComplexJoint : ComplexJointIndex_t {
    //     //
    // };
    //
    // typedef struct JointPitchValue {
    //     JointValue_t pitch = JOINT_VALUE_MIN;
    // };
    // typedef struct JointYawValue {
    //     JointValue_t yaw = JOINT_VALUE_MIN;
    // };
    // typedef struct JointRollValue {
    //     JointValue_t roll = JOINT_VALUE_MIN;
    // };
    // typedef struct JointPitchYawValue {
    //     JointValue_t pitch = JOINT_VALUE_MIN;
    //     JointValue_t yaw = JOINT_VALUE_MIN;
    // };
    // typedef struct JointPitchRollValue {
    //     JointValue_t pitch = JOINT_VALUE_MIN;
    //     JointValue_t roll = JOINT_VALUE_MIN;
    // };
    // typedef struct JointYawRollValue {
    //     JointValue_t yaw = JOINT_VALUE_MIN;
    //     JointValue_t roll = JOINT_VALUE_MIN;
    // };
    // typedef struct JointPitchYawRollValue {
    //     JointValue_t pitch = JOINT_VALUE_MIN;
    //     JointValue_t yaw = JOINT_VALUE_MIN;
    //     JointValue_t roll = JOINT_VALUE_MIN;
    // };
} // namespace SenseShift::Body::Skeleton
