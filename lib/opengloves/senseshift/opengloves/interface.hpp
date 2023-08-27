#pragma once

#include <cstdint>
#include <functional>
#include <variant>

#include <senseshift/body/skeleton/interface.hpp>

namespace SenseShift::OpenGloves {
    typedef uint16_t JointIndex_t;
    typedef enum class Joint : JointIndex_t {
        ThumbCurl,
        ThumbSplay,
        ThumbMCP_Curl,
        ThumbIP_Curl,

        IndexCurl,
        IndexSplay,
        IndexMCP_Curl,
        IndexPIP_Curl,
        IndexDIP_Curl,

        MiddleCurl,
        MiddleSplay,
        MiddleMCP_Curl,
        MiddlePIP_Curl,
        MiddleDIP_Curl,

        RingCurl,
        RingSplay,
        RingMCP_Curl,
        RingPIP_Curl,
        RingDIP_Curl,

        LittleCurl,
        LittleSplay,
        LittleMCP_Curl,
        LittlePIP_Curl,
        LittleDIP_Curl,
    } Joint_t;

    typedef uint16_t AnalogSensorTypeIndex_t;
    typedef enum class AnalogSensorType : AnalogSensorTypeIndex_t {
        JoystickX,
        JoystickY,
    } AnalogSensorType_t;

    typedef uint16_t DigitalSensorTypeIndex_t;
    typedef enum class DigitalSensorType : DigitalSensorTypeIndex_t {
        JoystickButton,

        ButtonA,
        ButtonB,
        ButtonMenu,
        ButtonCalibrate,

        GestureTrigger,
        GestureGrab,
        GesturePinch,
    } DigitalSensorType_t;

    typedef std::variant<Joint_t, AnalogSensorType_t, DigitalSensorType_t> Sensor_t;

    typedef std::uint16_t AnalogSensorValue_t;
    static const AnalogSensorValue_t ANALOG_SENSOR_VALUE_MIN = 0x0000;
    static const AnalogSensorValue_t ANALOG_SENSOR_VALUE_MAX = 0xFFFF;

    typedef std::variant<
      std::tuple<Joint_t, AnalogSensorValue_t>,
      std::tuple<AnalogSensorType_t, AnalogSensorValue_t>,
      DigitalSensorType_t>
      Command_t;

    typedef std::function<Command_t> CommandCallback_t;
} // namespace SenseShift::OpenGloves
