#include <opengloves/opengloves.hpp>
#include <unity.h>

using namespace og;

void test_encode_input_peripherals(void)
{
    const IEncoder* encoder = new AlphaEncoder();

    const std::vector<std::tuple<InputPeripheralData, std::string>> cases = {
        {
            InputPeripheralData({
                .curl = {
                    .thumb = {
                        .curl_total = 0.5,
                    },
                    .index = {
                        .curl_total = 0.5,
                    },
                    .middle = {
                        .curl_total = 0.5,
                    },
                    .ring = {
                        .curl_total = 0.5,
                    },
                    .pinky = {
                        .curl_total = 0.5,
                    },
                },
            }),
            "A2047B2047C2047D2047E2047\n",
        },
        {
            InputPeripheralData({
                .curl = {
                    .thumb = {
                        .curl_total = 0.0,
                    },
                    .index = {
                        .curl_total = 0.0,
                    },
                    .middle = {
                        .curl_total = 0.0,
                    },
                    .ring = {
                        .curl_total = 0.0,
                    },
                    .pinky = {
                        .curl_total = 0.0,
                    },
                },
            }),
            "A0B0C0D0E0\n",
        },
        {
            InputPeripheralData({
                .curl = {
                    .thumb = {
                        .curl_total = 1.0,
                    },
                    .index = {
                        .curl_total = 1.0,
                    },
                    .middle = {
                        .curl_total = 1.0,
                    },
                    .ring = {
                        .curl_total = 1.0,
                    },
                    .pinky = {
                        .curl_total = 1.0,
                    },
                },
            }),
            "A4095B4095C4095D4095E4095\n",
        },
        {
          InputPeripheralData({
                .splay = {
                    .thumb = 0.5,
                    .index = 0.5,
                    .middle = 0.5,
                    .ring = 0.5,
                    .pinky = 0.5,
                },
            }),
            "A0(AB)2047B0(BB)2047C0(CB)2047D0(DB)2047E0(EB)2047\n",
        },
        {
          InputPeripheralData({
            .curl = {
              .thumb = {
                .curl = { 0.25f, 0.5f, 0.75f, 1.0f },
              },
              .index = {
                .curl = { 0.25f, 0.5f, 0.75f, 1.0f },
              },
              .middle = {
                .curl = { 0.25f, 0.5f, 0.75f, 1.0f },
              },
              .ring = {
                .curl = { 0.25f, 0.5f, 0.75f, 1.0f },
              },
              .pinky = {
                .curl = { 0.25f, 0.5f, 0.75f, 1.0f },
              },
            },
          }),
          "A1023(AAB)2047(AAC)3071(AAD)4095B1023(BAB)2047(BAC)3071(BAD)4095C1023(CAB)2047(CAC)3071(CAD)4095D1023(DAB)2047(DAC)3071(DAD)4095E1023(EAB)2047(EAC)3071(EAD)4095\n",
        },
        {
          InputPeripheralData({
            .joystick = {
              .x = 0.5,
              .y = 0.5,
              .press = true,
            },
          }),
          "A0B0C0D0E0F2047G2047H\n"
        },
        {
          InputPeripheralData{
            .button_a = { .press = true },
            .button_calibrate = { .press = true },
          },
          "A0B0C0D0E0JO\n"
        },
        {
          InputPeripheralData{
            .pinch = { true },
            .grab = { true },
          },
          "A0B0C0D0E0MI\n"
        },
        {
          InputPeripheralData({
                .curl = {
                    .thumb = {
                        .curl_total = 0.5,
                    },
                    .index = {
                        .curl_total = 0.5,
                    },
                    .middle = {
                        .curl_total = 0.5,
                    },
                    .ring = {
                        .curl_total = 0.5,
                    },
                    .pinky = {
                        .curl_total = 0.5,
                    },
                },
                .splay = {
                    .thumb = 0.5,
                    .index = 0.5,
                    .middle = 0.5,
                    .ring = 0.5,
                    .pinky = 0.5,
                },
          }),
          "A2047(AB)2047B2047(BB)2047C2047(CB)2047D2047(DB)2047E2047(EB)2047\n",
        }
    };

    for (const auto& [data, expected] : cases) {
        const auto encoded = encoder->encode_input(data);
        TEST_ASSERT_EQUAL_STRING(expected.c_str(), encoded.c_str());
    }
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_encode_input_peripherals);

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
