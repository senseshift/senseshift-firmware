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

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(testDecode);

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
