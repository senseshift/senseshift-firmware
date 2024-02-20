#include <opengloves/opengloves.hpp>
#include <unity.h>

using namespace og;

void test_encode_input_peripherals(void)
{
    const IEncoder* encoder = new AlphaEncoder();

    const auto defaultEmpty = InputPeripheralData();

    auto halfCurl = InputPeripheralData();
    halfCurl.curl = {
        .thumb = { .curl_total = 0.5 },
        .index = { .curl_total = 0.5 },
        .middle = { .curl_total = 0.5 },
        .ring = { .curl_total = 0.5 },
        .pinky = { .curl_total = 0.5 },
    };

    auto zeroCurl = InputPeripheralData();
    zeroCurl.curl = {
        .thumb = { .curl_total = 0.0 },
        .index = { .curl_total = 0.0 },
        .middle = { .curl_total = 0.0 },
        .ring = { .curl_total = 0.0 },
        .pinky = { .curl_total = 0.0 },
    };

    auto fullCurl = InputPeripheralData();
    fullCurl.curl = {
        .thumb = { .curl_total = 1.0 },
        .index = { .curl_total = 1.0 },
        .middle = { .curl_total = 1.0 },
        .ring = { .curl_total = 1.0 },
        .pinky = { .curl_total = 1.0 },
    };

    auto halfSplay = InputPeripheralData();
    halfSplay.splay = {
        .thumb = 0.5,
        .index = 0.5,
        .middle = 0.5,
        .ring = 0.5,
        .pinky = 0.5,
    };

    auto increasingJoints = InputPeripheralData();
    increasingJoints.curl = {
        .thumb = { .curl = { 0.25f, 0.5f, 0.75f, 1.0f } },
        .index = { .curl = { 0.25f, 0.5f, 0.75f, 1.0f } },
        .middle = { .curl = { 0.25f, 0.5f, 0.75f, 1.0f } },
        .ring = { .curl = { 0.25f, 0.5f, 0.75f, 1.0f } },
        .pinky = { .curl = { 0.25f, 0.5f, 0.75f, 1.0f } },
    };

    auto joystick = InputPeripheralData();
    joystick.joystick = {
        .x = 0.5,
        .y = 0.5,
        .press = true,
    };

    auto buttons = InputPeripheralData();
    buttons.button_a = { .press = true };
    buttons.button_calibrate = { .press = true };

    auto gesture = InputPeripheralData();
    gesture.pinch = { true };
    gesture.grab = { true };

    auto halfCurlSplay = InputPeripheralData(halfCurl);
    halfCurlSplay.curl = {
        .thumb = { .curl_total = 0.5 },
        .index = { .curl_total = 0.5 },
        .middle = { .curl_total = 0.5 },
        .ring = { .curl_total = 0.5 },
        .pinky = { .curl_total = 0.5 },
    };
    halfCurlSplay.splay = {
        .thumb = 0.5,
        .index = 0.5,
        .middle = 0.5,
        .ring = 0.5,
        .pinky = 0.5,
    };

    const std::vector<std::tuple<InputPeripheralData, std::string>> cases = {
        {
          defaultEmpty,
          "A0B0C0D0E0\n",
        },
        {
          halfCurl,
          "A2047B2047C2047D2047E2047\n",
        },
        {
          zeroCurl,
          "A0B0C0D0E0\n",
        },
        {
          fullCurl,
          "A4095B4095C4095D4095E4095\n",
        },
        {
          halfSplay,
          "A0(AB)2047B0(BB)2047C0(CB)2047D0(DB)2047E0(EB)2047\n",
        },
        {
          increasingJoints,
          "A1023(AAB)2047(AAC)3071(AAD)4095B1023(BAB)2047(BAC)3071(BAD)4095C1023(CAB)2047(CAC)3071(CAD)4095D1023(DAB)2047(DAC)3071(DAD)4095E1023(EAB)2047(EAC)3071(EAD)4095\n",
        },
        {
          joystick,
          "A0B0C0D0E0F2047G2047H\n",
        },
        {
          buttons,
          "A0B0C0D0E0JO\n",
        },
        {
          gesture,
          "A0B0C0D0E0ML\n",
        },
        {
          halfCurlSplay,
          "A2047(AB)2047B2047(BB)2047C2047(CB)2047D2047(DB)2047E2047(EB)2047\n",
        }
    };

    for (auto i = 0; i < cases.size(); i++) {
        const auto [data, expected] = cases[i];
        const auto encoded = encoder->encode_input(data);
        TEST_ASSERT_EQUAL_STRING_MESSAGE(
          expected.c_str(),
          encoded.c_str(),
          ("Failed case " + std::to_string(i)).c_str()
        );
    }
}

void test_decode_output_ffb(void)
{
    const IEncoder* encoder = new AlphaEncoder();

    std::map<std::string, OutputForceFeedbackData> cases = {
        {
          "A0B0C0D0E0\n",
          OutputForceFeedbackData{
            .thumb = 0.0f,
            .index = 0.0f,
            .middle = 0.0f,
            .ring = 0.0f,
            .pinky = 0.0f,
          },
        },
        {
          "A0\n",
          OutputForceFeedbackData{
            .thumb = 0.0f,
            .index = 0.0f,
            .middle = 0.0f,
            .ring = 0.0f,
            .pinky = 0.0f,
          },
        },
        {
          "A819B1638C2457D3276E4095\n",
          OutputForceFeedbackData{
            .thumb = 0.2f,
            .index = 0.4f,
            .middle = 0.6f,
            .ring = 0.8f,
            .pinky = 1.0f,
          },
        },
        {
          "A4095B4095C4095D4095E4095\n",
          OutputForceFeedbackData{
            .thumb = 1.0f,
            .index = 1.0f,
            .middle = 1.0f,
            .ring = 1.0f,
            .pinky = 1.0f,
          },
        },
    };

    for (const auto& [data, expected] : cases) {
        const auto decoded = encoder->decode_output(data.c_str(), data.size());
        TEST_ASSERT_TRUE(std::holds_alternative<OutputForceFeedbackData>(decoded));
        TEST_ASSERT_TRUE(std::get<OutputForceFeedbackData>(decoded) == expected);
    }
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_encode_input_peripherals);
    RUN_TEST(test_decode_output_ffb);

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
