#include <senseshift/bh/devices.hpp>
#include <senseshift/bh/encoding.hpp>
#include <unity.h>

using namespace SenseShift::BH;
using namespace SenseShift::Body::Haptics;
using namespace SenseShift::Output;

class TestActuator : public IOutput<float> {
  public:
    bool isSetup = false;
    float intensity = 0;

    TestActuator() : IFloatOutput()
    {
    }
    void init() override
    {
        this->isSetup = true;
    }
    void writeState(float newIntensity) override
    {
        this->intensity = newIntensity;
    }
};

#define ASSERT_EQUAL_FLOAT_ROUNDED(expected, actual, precision)                \
    TEST_ASSERT_EQUAL_FLOAT(                                                   \
      std::round(expected* std::pow(10, precision)) / std::pow(10, precision), \
      std::round(actual * std::pow(10, precision)) / std::pow(10, precision)   \
    )

void test_layout_tactsuitx16(void)
{
    static const std::array<OutputLayout, BH_LAYOUT_TACTSUITX16_SIZE> bhLayout = { BH_LAYOUT_TACTSUITX16 };
    static const std::array<std::uint8_t, BH_LAYOUT_TACTSUITX16_GROUPS_SIZE> layoutGroups =
      BH_LAYOUT_TACTSUITX16_GROUPS;

    auto body = new FloatBody();

    TestActuator* actuator0 = new TestActuator();
    TestActuator* actuator1 = new TestActuator();
    TestActuator* actuator2 = new TestActuator();
    TestActuator* actuator3 = new TestActuator();
    TestActuator* actuator4 = new TestActuator();
    TestActuator* actuator5 = new TestActuator();
    TestActuator* actuator6 = new TestActuator();
    TestActuator* actuator7 = new TestActuator();
    TestActuator* actuator8 = new TestActuator();
    TestActuator* actuator9 = new TestActuator();
    TestActuator* actuator10 = new TestActuator();
    TestActuator* actuator11 = new TestActuator();
    TestActuator* actuator12 = new TestActuator();
    TestActuator* actuator13 = new TestActuator();
    TestActuator* actuator14 = new TestActuator();
    TestActuator* actuator15 = new TestActuator();

    auto frontOutputs = PlaneMapper_Margin::mapMatrixCoordinates<FloatPlane::Actuator*>({
      { actuator0, actuator1, actuator2, actuator3 },
      { actuator4, actuator5, actuator6, actuator7 },
    });
    auto backOutputs = PlaneMapper_Margin::mapMatrixCoordinates<FloatPlane::Actuator*>({
      { actuator8, actuator9, actuator10, actuator11 },
      { actuator12, actuator13, actuator14, actuator15 },
    });

    auto frontPlane = new FloatPlane(frontOutputs);
    auto backPlane = new FloatPlane(backOutputs);

    body->addTarget(Target::ChestFront, frontPlane);
    body->addTarget(Target::ChestBack, backPlane);
    // body->setup();

    const std::array<uint8_t, 20> values = {
        0x01, 0x00, 0x23, 0x00, 0x00, 0x45, 0x00, 0x67, 0x00, 0x00,
        0x89, 0x00, 0xab, 0x00, 0x00, 0xcd, 0x00, 0xef, 0x00, 0x00,
    };

    Decoder::applyVestGrouped(body, values, bhLayout, layoutGroups);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, actuator0->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(273.0F / 4095.0F, actuator1->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(3276.0F / 4095.0F, actuator2->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(3549.0F / 4095.0F, actuator3->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(546.0F / 4095.0F, actuator4->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(819.0F / 4095.0F, actuator5->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(3822.0F / 4095.0F, actuator6->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(4095.0F / 4095.0F, actuator7->intensity, 2);

    ASSERT_EQUAL_FLOAT_ROUNDED(1092.0F / 4095.0F, actuator8->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(1365.0F / 4095.0F, actuator9->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(2184.0F / 4095.0F, actuator10->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(2457.0F / 4095.0F, actuator11->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(1638.0F / 4095.0F, actuator12->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(1911.0F / 4095.0F, actuator13->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(2730.0F / 4095.0F, actuator14->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(3003.0F / 4095.0F, actuator15->intensity, 2);
}

void test_layout_tactsuitx40(void)
{
    static const std::array<OutputLayout, BH_LAYOUT_TACTSUITX40_SIZE> bhLayout = { BH_LAYOUT_TACTSUITX40 };

    auto body = new FloatBody();

    std::vector<std::vector<FloatPlane::Actuator*>> frontMatrix = {
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
    };
    std::vector<std::vector<FloatPlane::Actuator*>> backMatrix = {
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
    };

    auto frontOutputs = PlaneMapper_Margin::mapMatrixCoordinates<FloatPlane::Actuator*>(frontMatrix);
    auto backOutputs = PlaneMapper_Margin::mapMatrixCoordinates<FloatPlane::Actuator*>(backMatrix);

    auto frontPlane = new FloatPlane(frontOutputs);
    auto backPlane = new FloatPlane(backOutputs);

    body->addTarget(Target::ChestFront, frontPlane);
    body->addTarget(Target::ChestBack, backPlane);
    // body->setup();

    Decoder::applyVest(
      body,
      {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      },
      bhLayout
    );
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(frontMatrix[0][0])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(273.0F / 4095.0F, static_cast<TestActuator*>(frontMatrix[0][1])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(frontMatrix[0][2])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(frontMatrix[0][3])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(546.0F / 4095.0F, static_cast<TestActuator*>(frontMatrix[1][0])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(819.0F / 4095.0F, static_cast<TestActuator*>(frontMatrix[1][1])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(frontMatrix[1][2])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(frontMatrix[1][3])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(1092.0F / 4095.0F, static_cast<TestActuator*>(frontMatrix[2][0])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(1365.0F / 4095.0F, static_cast<TestActuator*>(frontMatrix[2][1])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(frontMatrix[2][2])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(frontMatrix[2][3])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(1638.0F / 4095.0F, static_cast<TestActuator*>(frontMatrix[3][0])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(1911.0F / 4095.0F, static_cast<TestActuator*>(frontMatrix[3][1])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(frontMatrix[3][2])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(frontMatrix[3][3])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(2184.0F / 4095.0F, static_cast<TestActuator*>(frontMatrix[4][0])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(2457.0F / 4095.0F, static_cast<TestActuator*>(frontMatrix[4][1])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(frontMatrix[4][2])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(frontMatrix[4][3])->intensity, 2);

    ASSERT_EQUAL_FLOAT_ROUNDED(2730.0F / 4095.0F, static_cast<TestActuator*>(backMatrix[0][0])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(3003.0F / 4095.0F, static_cast<TestActuator*>(backMatrix[0][1])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(backMatrix[0][2])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(backMatrix[0][3])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(3276.0F / 4095.0F, static_cast<TestActuator*>(backMatrix[1][0])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(3549.0F / 4095.0F, static_cast<TestActuator*>(backMatrix[1][1])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(backMatrix[1][2])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(backMatrix[1][3])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(3822.0F / 4095.0F, static_cast<TestActuator*>(backMatrix[2][0])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(4095.0F / 4095.0F, static_cast<TestActuator*>(backMatrix[2][1])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(backMatrix[2][2])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(backMatrix[2][3])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(backMatrix[3][0])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(backMatrix[3][1])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(backMatrix[3][2])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(backMatrix[3][3])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(backMatrix[4][0])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(backMatrix[4][1])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(backMatrix[4][2])->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, static_cast<TestActuator*>(backMatrix[4][3])->intensity, 2);
}

void test_layout_tactal(void)
{
    static const std::array<Position, BH_LAYOUT_TACTAL_SIZE> bhLayout = { BH_LAYOUT_TACTAL };

    auto body = new FloatBody();

    TestActuator* actuator0 = new TestActuator();
    TestActuator* actuator1 = new TestActuator();
    TestActuator* actuator2 = new TestActuator();
    TestActuator* actuator3 = new TestActuator();
    TestActuator* actuator4 = new TestActuator();
    TestActuator* actuator5 = new TestActuator();

    auto outputs = PlaneMapper_Margin::mapMatrixCoordinates<FloatPlane::Actuator*>({
      { actuator0, actuator1, actuator2, actuator3, actuator4, actuator5 },
    });
    auto plane = new FloatPlane(outputs);

    body->addTarget(Target::FaceFront, plane);

    Decoder::applyPlain(body, { 0x64, 0x00, 0x00, 0x00, 0x00, 0x00 }, bhLayout, Effect::Vibro, Target::FaceFront);
    ASSERT_EQUAL_FLOAT_ROUNDED(1.0F, actuator0->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, actuator1->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, actuator2->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, actuator3->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, actuator4->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, actuator5->intensity, 2);

    Decoder::applyPlain(body, { 0x10, 0x20, 0x30, 0x40, 0x50, 0x60 }, bhLayout, Effect::Vibro, Target::FaceFront);
    ASSERT_EQUAL_FLOAT_ROUNDED(0.16F, actuator0->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(1310.0F / 4095.0F, actuator1->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(1965.0F / 4095.0F, actuator2->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(2620.0F / 4095.0F, actuator3->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(3276.0F / 4095.0F, actuator4->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(3931.0F / 4095.0F, actuator5->intensity, 2);
}

void test_layout_tactglove(void)
{
    TestActuator* actuatorThumb = new TestActuator();
    TestActuator* actuatorIndex = new TestActuator();
    TestActuator* actuatorMiddle = new TestActuator();
    TestActuator* actuatorRing = new TestActuator();
    TestActuator* actuatorLittle = new TestActuator();
    TestActuator* actuatorWrist = new TestActuator();

    auto body = new FloatBody();
    const auto& bhLayout = TactGloveLeftLayout;

    addTactGloveActuators(
      body,
      ::SenseShift::Body::Hands::HandSide::Left,
      actuatorThumb,
      actuatorIndex,
      actuatorMiddle,
      actuatorRing,
      actuatorLittle,
      actuatorWrist
    );

    Decoder::applyPlain(body, { 0x64, 0x00, 0x00, 0x00, 0x00, 0x00 }, bhLayout, Effect::Vibro);
    ASSERT_EQUAL_FLOAT_ROUNDED(4095.0F / 4095.0F, actuatorThumb->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, actuatorIndex->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, actuatorMiddle->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, actuatorRing->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, actuatorLittle->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(0, actuatorWrist->intensity, 2);

    Decoder::applyPlain(body, { 0x10, 0x20, 0x30, 0x40, 0x50, 0x60 }, bhLayout, Effect::Vibro);
    ASSERT_EQUAL_FLOAT_ROUNDED(655.0F / 4095.0F, actuatorThumb->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(1310.0F / 4095.0F, actuatorIndex->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(1965.0F / 4095.0F, actuatorMiddle->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(2620.0F / 4095.0F, actuatorRing->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(3276.0F / 4095.0F, actuatorLittle->intensity, 2);
    ASSERT_EQUAL_FLOAT_ROUNDED(3931.0F / 4095.0F, actuatorWrist->intensity, 2);
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_layout_tactsuitx16);
    RUN_TEST(test_layout_tactsuitx40);
    RUN_TEST(test_layout_tactal);
    RUN_TEST(test_layout_tactglove);

    return UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>

void setup(void)
{
    process();
}

void loop(void)
{
}

#else

int main(int argc, char** argv)
{
    return process();
}

#endif
