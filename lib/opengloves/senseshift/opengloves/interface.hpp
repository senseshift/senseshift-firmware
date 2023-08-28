#pragma once

#include <cstdint>
#include <functional>
#include <variant>

#include <senseshift/body/skeleton/interface.hpp>

namespace SenseShift::OpenGloves {
    typedef std::uint16_t AnalogSensorValue_t;
    static const AnalogSensorValue_t ANALOG_SENSOR_VALUE_MIN = 0x0000;
    static const AnalogSensorValue_t ANALOG_SENSOR_VALUE_MAX = 0xFFFF;

    typedef uint16_t JointIndex_t;
    enum class Joint : JointIndex_t {
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
    };
    typedef std::tuple<Joint, AnalogSensorValue_t> JointCommand_t;

    typedef uint16_t AnalogSensorTypeIndex_t;
    enum class AnalogSensorType : AnalogSensorTypeIndex_t {
        JoystickX,
        JoystickY,

        Trigger,
    };
    typedef std::tuple<AnalogSensorType, AnalogSensorValue_t> AnalogSensorCommand_t;

    typedef uint16_t DigitalSensorTypeIndex_t;
    enum class DigitalSensorType : DigitalSensorTypeIndex_t {
        JoystickButton,

        ButtonA,
        ButtonB,
        ButtonMenu,
        ButtonCalibrate,

        GestureTrigger,
        GestureGrab,
        GesturePinch,
    };

    typedef std::variant<Joint, AnalogSensorType, DigitalSensorType> Sensor_t;

    typedef std::variant<JointCommand_t, AnalogSensorCommand_t, DigitalSensorType> Command_t;

    typedef std::function<Command_t> CommandCallback_t;

    class ITransport {
      public:
        virtual void setup() = 0;
        virtual size_t send(char* buffer, size_t length) = 0;
        virtual bool hasData() = 0;
        virtual size_t read(char* buffer, size_t length) = 0;
    };
    struct ISerializer {
        virtual std::string serialize(const std::vector<Command_t>& commands) const = 0;
        virtual std::vector<Command_t> deserialize(const std::string& buffer) const = 0;
    };
} // namespace SenseShift::OpenGloves
