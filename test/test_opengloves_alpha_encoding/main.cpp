#include <senseshift/opengloves/encoding.hpp>
#include <unity.h>

using namespace SenseShift::OpenGloves;

void testDecode(void)
{
    std::map<std::string, std::vector<Command_t>> input_strings = {
        // curl only
        {
          "A2048\n\0",
          {
            std::make_pair(Joint::ThumbCurl, 2048),
          },
        },
        // curl only
        {
          "A100B200C300D400E500",
          {
            std::make_pair(Joint::ThumbCurl, 100),
            std::make_pair(Joint::IndexCurl, 200),
            std::make_pair(Joint::MiddleCurl, 300),
            std::make_pair(Joint::RingCurl, 400),
            std::make_pair(Joint::LittleCurl, 500),
          },
        },
        // curl (unordered)
        {
          "E500A100B200D400C300",
          {
            std::make_pair(Joint::ThumbCurl, 100),
            std::make_pair(Joint::IndexCurl, 200),
            std::make_pair(Joint::MiddleCurl, 300),
            std::make_pair(Joint::RingCurl, 400),
            std::make_pair(Joint::LittleCurl, 500),
          },
        },
        // curl (with invalid data)
        {
          "A100B200C300D400E",
          {
            std::make_pair(Joint::ThumbCurl, 100),
            std::make_pair(Joint::IndexCurl, 200),
            std::make_pair(Joint::MiddleCurl, 300),
            std::make_pair(Joint::RingCurl, 400),
          },
        },
        // curl (with unknown prefix)
        {
          "X100A100B200C300D400E500",
          {
            std::make_pair(Joint::ThumbCurl, 100),
            std::make_pair(Joint::IndexCurl, 200),
            std::make_pair(Joint::MiddleCurl, 300),
            std::make_pair(Joint::RingCurl, 400),
            std::make_pair(Joint::LittleCurl, 500),
          },
        },
        // curl + splay
        {
          "A1(AB)2B3(BB)4C5(CB)6D7(DB)8E9(EB)10",
          {
            std::make_pair(Joint::ThumbCurl, 1),
            std::make_pair(Joint::ThumbSplay, 2),
            std::make_pair(Joint::IndexCurl, 3),
            std::make_pair(Joint::IndexSplay, 4),
            std::make_pair(Joint::MiddleCurl, 5),
            std::make_pair(Joint::MiddleSplay, 6),
            std::make_pair(Joint::RingCurl, 7),
            std::make_pair(Joint::RingSplay, 8),
            std::make_pair(Joint::LittleCurl, 9),
            std::make_pair(Joint::LittleSplay, 10),
          },
        },
        // curl + splay (unordered)
        {
          "E9A1B3D7C5(BB)4(AB)2(EB)10(CB)6(DB)8",
          {
            std::make_pair(Joint::ThumbCurl, 1),
            std::make_pair(Joint::ThumbSplay, 2),
            std::make_pair(Joint::IndexCurl, 3),
            std::make_pair(Joint::IndexSplay, 4),
            std::make_pair(Joint::MiddleCurl, 5),
            std::make_pair(Joint::MiddleSplay, 6),
            std::make_pair(Joint::RingCurl, 7),
            std::make_pair(Joint::RingSplay, 8),
            std::make_pair(Joint::LittleCurl, 9),
            std::make_pair(Joint::LittleSplay, 10),
          },
        },
    };

    for (auto& [input_string, expected_commands] : input_strings) {
        auto commands = AlphaEncodingService::decode(input_string);

        TEST_ASSERT_EQUAL_MESSAGE(
          expected_commands.size(),
          commands.size(),
          "Expected commands size does not match actual commands size"
        );

        for (auto& command : expected_commands) {
            // find the command in the actual commands
            auto it = std::find(commands.begin(), commands.end(), command);
            TEST_ASSERT_MESSAGE(it != commands.end(), "Command not found in actual commands");
        }
    }
}

void testEncode(void)
{
    std::map<std::string, std::vector<Command_t>> expectations = {
        {
          "A2048",
          {
            std::make_pair(Joint::ThumbCurl, 2048),
          },
        },
        {
          "A10B20C30D40E50",
          {
            std::make_pair(Joint::ThumbCurl, 10),
            std::make_pair(Joint::IndexCurl, 20),
            std::make_pair(Joint::MiddleCurl, 30),
            std::make_pair(Joint::RingCurl, 40),
            std::make_pair(Joint::LittleCurl, 50),
          },
        },
        {
          "A10B20C30D40E50F60G70HIJKLMNO",
          {
            std::make_pair(Joint::ThumbCurl, 10),
            std::make_pair(Joint::IndexCurl, 20),
            std::make_pair(Joint::MiddleCurl, 30),
            std::make_pair(Joint::RingCurl, 40),
            std::make_pair(Joint::LittleCurl, 50),
            std::make_pair(AnalogSensorType::JoystickX, 60),
            std::make_pair(AnalogSensorType::JoystickY, 70),
            DigitalSensorType::JoystickButton,
            DigitalSensorType::GestureTrigger,
            DigitalSensorType::ButtonA,
            DigitalSensorType::ButtonB,
            DigitalSensorType::GestureGrab,
            DigitalSensorType::GesturePinch,
            DigitalSensorType::ButtonMenu,
            DigitalSensorType::ButtonCalibrate,
          },
        },
        {
          "A10(AB)20(AAA)30(AAB)40B50(BB)60(BAA)70(BAB)80(BAC)90C100(CB)110(CAA)120(CAB)130(CAC)140D150(DB)160(DAA)170(DAB)180(DAC)190E200(EB)210(EAA)220(EAB)230(EAC)240",
          {
            std::make_pair(Joint::ThumbCurl, 10),       std::make_pair(Joint::ThumbSplay, 20),
            std::make_pair(Joint::ThumbMCP_Curl, 30),   std::make_pair(Joint::ThumbIP_Curl, 40),

            std::make_pair(Joint::IndexCurl, 50),       std::make_pair(Joint::IndexSplay, 60),
            std::make_pair(Joint::IndexMCP_Curl, 70),   std::make_pair(Joint::IndexPIP_Curl, 80),
            std::make_pair(Joint::IndexDIP_Curl, 90),

            std::make_pair(Joint::MiddleCurl, 100),     std::make_pair(Joint::MiddleSplay, 110),
            std::make_pair(Joint::MiddleMCP_Curl, 120), std::make_pair(Joint::MiddlePIP_Curl, 130),
            std::make_pair(Joint::MiddleDIP_Curl, 140),

            std::make_pair(Joint::RingCurl, 150),       std::make_pair(Joint::RingSplay, 160),
            std::make_pair(Joint::RingMCP_Curl, 170),   std::make_pair(Joint::RingPIP_Curl, 180),
            std::make_pair(Joint::RingDIP_Curl, 190),

            std::make_pair(Joint::LittleCurl, 200),     std::make_pair(Joint::LittleSplay, 210),
            std::make_pair(Joint::LittleMCP_Curl, 220), std::make_pair(Joint::LittlePIP_Curl, 230),
            std::make_pair(Joint::LittleDIP_Curl, 240),
          },
        },
    };

    for (auto& [expected_string, commands] : expectations) {
        auto actual_string = AlphaEncodingService::encode(commands);
        TEST_ASSERT_EQUAL_STRING_MESSAGE(
          expected_string.c_str(),
          actual_string.c_str(),
          "Encoded string does not match expected string"
        );
    }
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(testDecode);
    RUN_TEST(testEncode);

    return UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>

void setup(void)
{
    process();
}

void loop(void) {}

#else

int main(int argc, char** argv)
{
    return process();
}

#endif
