#include <senseshift/bh/devices.hpp>
#include <senseshift/bh/encoding.hpp>
#include <unity.h>

using namespace SenseShift::BH;
using namespace SenseShift::Body::Haptics;
using namespace SenseShift::Output;

class TestActuator : public IActuator<uint16_t> {
  public:
    bool isSetup = false;
    uint16_t intensity = 0;

    TestActuator() : IActuator<uint16_t>() {}
    void setup() override { this->isSetup = true; }
    void writeOutput(uint16_t intensity) override { this->intensity = intensity; }
};

void test_layout_tactsuitx16(void)
{
    static const size_t bhLayoutSize = BH_LAYOUT_TACTSUITX16_SIZE;
    static const OutputLayout_t bhLayout[bhLayoutSize] = BH_LAYOUT_TACTSUITX16;

    static const size_t layoutGroupsSize = BH_LAYOUT_TACTSUITX16_GROUPS_SIZE;
    static const uint8_t layoutGroups[layoutGroupsSize] = BH_LAYOUT_TACTSUITX16_GROUPS;

    auto body = new HapticBody();

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

    auto frontOutputs = PlaneMapper_Margin::mapMatrixCoordinates<VibroPlane::Actuator_t>({
      { actuator0, actuator1, actuator2, actuator3 },
      { actuator4, actuator5, actuator6, actuator7 },
    });
    auto backOutputs = PlaneMapper_Margin::mapMatrixCoordinates<VibroPlane::Actuator_t>({
      { actuator8, actuator9, actuator10, actuator11 },
      { actuator12, actuator13, actuator14, actuator15 },
    });

    auto frontPlane = new VibroPlane(frontOutputs);
    auto backPlane = new VibroPlane(backOutputs);

    body->addTarget(Target::ChestFront, frontPlane);
    body->addTarget(Target::ChestBack, backPlane);
    // body->setup();

    const uint8_t values[] = {
        0x01, 0x00, 0x23, 0x00, 0x00, 0x45, 0x00, 0x67, 0x00, 0x00,
        0x89, 0x00, 0xab, 0x00, 0x00, 0xcd, 0x00, 0xef, 0x00, 0x00,
    };

    Decoder::applyVestGrouped(body, values, bhLayout, layoutGroups);
    TEST_ASSERT_EQUAL_INT(0, actuator0->intensity);
    TEST_ASSERT_EQUAL_INT(273, actuator1->intensity);
    TEST_ASSERT_EQUAL_INT(3276, actuator2->intensity);
    TEST_ASSERT_EQUAL_INT(3549, actuator3->intensity);
    TEST_ASSERT_EQUAL_INT(546, actuator4->intensity);
    TEST_ASSERT_EQUAL_INT(819, actuator5->intensity);
    TEST_ASSERT_EQUAL_INT(3822, actuator6->intensity);
    TEST_ASSERT_EQUAL_INT(4095, actuator7->intensity);

    TEST_ASSERT_EQUAL_INT(1092, actuator8->intensity);
    TEST_ASSERT_EQUAL_INT(1365, actuator9->intensity);
    TEST_ASSERT_EQUAL_INT(2184, actuator10->intensity);
    TEST_ASSERT_EQUAL_INT(2457, actuator11->intensity);
    TEST_ASSERT_EQUAL_INT(1638, actuator12->intensity);
    TEST_ASSERT_EQUAL_INT(1911, actuator13->intensity);
    TEST_ASSERT_EQUAL_INT(2730, actuator14->intensity);
    TEST_ASSERT_EQUAL_INT(3003, actuator15->intensity);
}

void test_layout_tactsuitx40(void)
{
    static const size_t bhLayoutSize = BH_LAYOUT_TACTSUITX40_SIZE;
    static const OutputLayout_t bhLayout[bhLayoutSize] = BH_LAYOUT_TACTSUITX40;

    auto body = new HapticBody();

    std::vector<std::vector<VibroPlane::Actuator_t*>> frontMatrix = {
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
    };
    std::vector<std::vector<VibroPlane::Actuator_t*>> backMatrix = {
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
    };

    auto frontOutputs = PlaneMapper_Margin::mapMatrixCoordinates<VibroPlane::Actuator_t>(frontMatrix);
    auto backOutputs = PlaneMapper_Margin::mapMatrixCoordinates<VibroPlane::Actuator_t>(backMatrix);

    auto frontPlane = new VibroPlane(frontOutputs);
    auto backPlane = new VibroPlane(backOutputs);

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
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(frontMatrix[0][0])->intensity);
    TEST_ASSERT_EQUAL_INT(273, static_cast<TestActuator*>(frontMatrix[0][1])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(frontMatrix[0][2])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(frontMatrix[0][3])->intensity);
    TEST_ASSERT_EQUAL_INT(546, static_cast<TestActuator*>(frontMatrix[1][0])->intensity);
    TEST_ASSERT_EQUAL_INT(819, static_cast<TestActuator*>(frontMatrix[1][1])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(frontMatrix[1][2])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(frontMatrix[1][3])->intensity);
    TEST_ASSERT_EQUAL_INT(1092, static_cast<TestActuator*>(frontMatrix[2][0])->intensity);
    TEST_ASSERT_EQUAL_INT(1365, static_cast<TestActuator*>(frontMatrix[2][1])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(frontMatrix[2][2])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(frontMatrix[2][3])->intensity);
    TEST_ASSERT_EQUAL_INT(1638, static_cast<TestActuator*>(frontMatrix[3][0])->intensity);
    TEST_ASSERT_EQUAL_INT(1911, static_cast<TestActuator*>(frontMatrix[3][1])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(frontMatrix[3][2])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(frontMatrix[3][3])->intensity);
    TEST_ASSERT_EQUAL_INT(2184, static_cast<TestActuator*>(frontMatrix[4][0])->intensity);
    TEST_ASSERT_EQUAL_INT(2457, static_cast<TestActuator*>(frontMatrix[4][1])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(frontMatrix[4][2])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(frontMatrix[4][3])->intensity);

    TEST_ASSERT_EQUAL_INT(2730, static_cast<TestActuator*>(backMatrix[0][0])->intensity);
    TEST_ASSERT_EQUAL_INT(3003, static_cast<TestActuator*>(backMatrix[0][1])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(backMatrix[0][2])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(backMatrix[0][3])->intensity);
    TEST_ASSERT_EQUAL_INT(3276, static_cast<TestActuator*>(backMatrix[1][0])->intensity);
    TEST_ASSERT_EQUAL_INT(3549, static_cast<TestActuator*>(backMatrix[1][1])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(backMatrix[1][2])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(backMatrix[1][3])->intensity);
    TEST_ASSERT_EQUAL_INT(3822, static_cast<TestActuator*>(backMatrix[2][0])->intensity);
    TEST_ASSERT_EQUAL_INT(4095, static_cast<TestActuator*>(backMatrix[2][1])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(backMatrix[2][2])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(backMatrix[2][3])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(backMatrix[3][0])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(backMatrix[3][1])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(backMatrix[3][2])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(backMatrix[3][3])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(backMatrix[4][0])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(backMatrix[4][1])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(backMatrix[4][2])->intensity);
    TEST_ASSERT_EQUAL_INT(0, static_cast<TestActuator*>(backMatrix[4][3])->intensity);
}

void test_layout_tactal(void)
{
    static const size_t bhLayoutSize = BH_LAYOUT_TACTAL_SIZE;
    static const ::SenseShift::Body::Haptics::Position_t bhLayout[bhLayoutSize] = BH_LAYOUT_TACTAL;

    auto body = new HapticBody();

    TestActuator* actuator0 = new TestActuator();
    TestActuator* actuator1 = new TestActuator();
    TestActuator* actuator2 = new TestActuator();
    TestActuator* actuator3 = new TestActuator();
    TestActuator* actuator4 = new TestActuator();
    TestActuator* actuator5 = new TestActuator();

    auto outputs = PlaneMapper_Margin::mapMatrixCoordinates<VibroPlane::Actuator_t>({
      { actuator0, actuator1, actuator2, actuator3, actuator4, actuator5 },
    });
    auto plane = new VibroPlane(outputs);

    body->addTarget(Target::FaceFront, plane);

    Decoder::applyPlain(body, { 0x64, 0x00, 0x00, 0x00, 0x00, 0x00 }, bhLayout, Effect::Vibro, Target::FaceFront);
    TEST_ASSERT_EQUAL_INT(4095, actuator0->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuator1->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuator2->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuator3->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuator4->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuator5->intensity);

    Decoder::applyPlain(body, { 0x10, 0x20, 0x30, 0x40, 0x50, 0x60 }, bhLayout, Effect::Vibro, Target::FaceFront);
    TEST_ASSERT_EQUAL_INT(655, actuator0->intensity);
    TEST_ASSERT_EQUAL_INT(1310, actuator1->intensity);
    TEST_ASSERT_EQUAL_INT(1965, actuator2->intensity);
    TEST_ASSERT_EQUAL_INT(2620, actuator3->intensity);
    TEST_ASSERT_EQUAL_INT(3276, actuator4->intensity);
    TEST_ASSERT_EQUAL_INT(3931, actuator5->intensity);
}

void test_layout_tactglove(void)
{
    TestActuator* actuatorThumb = new TestActuator();
    TestActuator* actuatorIndex = new TestActuator();
    TestActuator* actuatorMiddle = new TestActuator();
    TestActuator* actuatorRing = new TestActuator();
    TestActuator* actuatorLittle = new TestActuator();
    TestActuator* actuatorWrist = new TestActuator();

    auto body = new HapticBody();
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
    TEST_ASSERT_EQUAL_INT(4095, actuatorThumb->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuatorIndex->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuatorMiddle->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuatorRing->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuatorLittle->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuatorWrist->intensity);

    Decoder::applyPlain(body, { 0x10, 0x20, 0x30, 0x40, 0x50, 0x60 }, bhLayout, Effect::Vibro);
    TEST_ASSERT_EQUAL_INT(655, actuatorThumb->intensity);
    TEST_ASSERT_EQUAL_INT(1310, actuatorIndex->intensity);
    TEST_ASSERT_EQUAL_INT(1965, actuatorMiddle->intensity);
    TEST_ASSERT_EQUAL_INT(2620, actuatorRing->intensity);
    TEST_ASSERT_EQUAL_INT(3276, actuatorLittle->intensity);
    TEST_ASSERT_EQUAL_INT(3931, actuatorWrist->intensity);
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

void loop(void) {}

#else

int main(int argc, char** argv)
{
    return process();
}

#endif
