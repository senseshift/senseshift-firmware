#include <og_alpha_encoding.hpp>
#include <unity.h>

using namespace OpenGloves;

void testSplitCommands(void)
{
    std::map<std::string, std::map<Command, int>> input_strings = {
        // curl only
        {
          "A100B200C300D400E500",
          {
            { Command::ThumbCurl, 100 },
            { Command::IndexCurl, 200 },
            { Command::MiddleCurl, 300 },
            { Command::RingCurl, 400 },
            { Command::PinkyCurl, 500 },
          },
        },
        // curl (unordered)
        {
          "E500A100B200D400C300",
          {
            { Command::ThumbCurl, 100 },
            { Command::IndexCurl, 200 },
            { Command::MiddleCurl, 300 },
            { Command::RingCurl, 400 },
            { Command::PinkyCurl, 500 },
          },
        },
        // curl (with invalid data)
        {
          "A100B200C300D400E",
          {
            { Command::ThumbCurl, 100 },
            { Command::IndexCurl, 200 },
            { Command::MiddleCurl, 300 },
            { Command::RingCurl, 400 },
          },
        },
        // curl (with unknown prefix)
        {
          "X100A100B200C300D400E500",
          {
            { Command::ThumbCurl, 100 },
            { Command::IndexCurl, 200 },
            { Command::MiddleCurl, 300 },
            { Command::RingCurl, 400 },
            { Command::PinkyCurl, 500 },
          },
        },
        // curl + splay
        {
          "A1(AB)2B3(BB)4C5(CB)6D7(DB)8E9(EB)10",
          {
            { Command::ThumbCurl, 1 },
            { Command::ThumbSplay, 2 },
            { Command::IndexCurl, 3 },
            { Command::IndexSplay, 4 },
            { Command::MiddleCurl, 5 },
            { Command::MiddleSplay, 6 },
            { Command::RingCurl, 7 },
            { Command::RingSplay, 8 },
            { Command::PinkyCurl, 9 },
            { Command::PinkySplay, 10 },
          },
        },
        // curl + splay (unordered)
        {
          "E9A1B3D7C5(BB)4(AB)2(EB)10(CB)6(DB)8",
          {
            { Command::ThumbCurl, 1 },
            { Command::ThumbSplay, 2 },
            { Command::IndexCurl, 3 },
            { Command::IndexSplay, 4 },
            { Command::MiddleCurl, 5 },
            { Command::MiddleSplay, 6 },
            { Command::RingCurl, 7 },
            { Command::RingSplay, 8 },
            { Command::PinkyCurl, 9 },
            { Command::PinkySplay, 10 },
          },
        },
    };

    for (auto& [input_string, expected_commands] : input_strings) {
        std::map<Command, uint16_t> commands = AlphaEncodingService::splitCommands(input_string);

        TEST_ASSERT_EQUAL_size_t_MESSAGE(
          expected_commands.size(),
          commands.size(),
          "Expected commands size does not match actual commands size"
        );

        for (auto& [command, value] : expected_commands) {
            TEST_ASSERT_EQUAL_INT(value, commands[command]);
        }
    }
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(testSplitCommands);

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
