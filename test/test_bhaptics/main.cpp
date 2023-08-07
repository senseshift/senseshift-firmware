#include <bh_types.hpp>
#include <bh_utils.hpp>
#include <haptic_plane.hpp>
#include <unity.h>

using namespace OH;
using namespace BH;

class TestActuator : public AbstractActuator {
  public:
    bool isSetup = false;
    oh_output_intensity_t intensity = 0;

    TestActuator() : AbstractActuator() {}
    void setup() override
    {
        this->isSetup = true;
    }
    void writeOutput(oh_output_intensity_t intensity) override
    {
        this->intensity = intensity;
    }
};

void test_layout_tactsuitx16(void)
{
    static const size_t bhLayoutSize = BH_LAYOUT_TACTSUITX16_SIZE;
    static const oh_output_point_t* bhLayout[bhLayoutSize] = BH_LAYOUT_TACTSUITX16;

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

    auto frontOutputs = PlaneMapper_Margin::mapMatrixCoordinates<AbstractActuator>({
      { actuator0, actuator1, actuator2, actuator3 },
      { actuator4, actuator5, actuator6, actuator7 },
    });
    auto backOutputs = PlaneMapper_Margin::mapMatrixCoordinates<AbstractActuator>({
      { actuator8, actuator9, actuator10, actuator11 },
      { actuator12, actuator13, actuator14, actuator15 },
    });

    auto frontPlane = new HapticPlane(frontOutputs);
    auto backPlane = new HapticPlane(backOutputs);

    body->addComponent(OUTPUT_PATH_CHEST_FRONT, frontPlane);
    body->addComponent(OUTPUT_PATH_CHEST_BACK, backPlane);
    body->setup();

    uint8_t values[] = {
        0x01, 0x00, 0x23, 0x00, 0x00, 0x45, 0x00, 0x67, 0x00, 0x00,
        0x89, 0x00, 0xab, 0x00, 0x00, 0xcd, 0x00, 0xef, 0x00, 0x00,
    };
    std::string value = std::string((char*) values, sizeof(values));

    vestX16OutputTransformer(body, value, bhLayout, bhLayoutSize, layoutGroups, layoutGroupsSize);
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
    static const oh_output_point_t* bhLayout[bhLayoutSize] = BH_LAYOUT_TACTSUITX40;

    auto body = new HapticBody();

    std::vector<std::vector<AbstractActuator*>> frontMatrix = {
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
    };
    std::vector<std::vector<AbstractActuator*>> backMatrix = {
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
        { new TestActuator(), new TestActuator(), new TestActuator(), new TestActuator() },
    };

    auto frontOutputs = PlaneMapper_Margin::mapMatrixCoordinates<AbstractActuator>(frontMatrix);
    auto backOutputs = PlaneMapper_Margin::mapMatrixCoordinates<AbstractActuator>(backMatrix);

    auto frontPlane = new HapticPlane(frontOutputs);
    auto backPlane = new HapticPlane(backOutputs);

    body->addComponent(OUTPUT_PATH_CHEST_FRONT, frontPlane);
    body->addComponent(OUTPUT_PATH_CHEST_BACK, backPlane);
    body->setup();

    uint8_t values[] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };
    std::string value = std::string((char*) values, sizeof(values));

    vestOutputTransformer(body, value, bhLayout, bhLayoutSize);
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
    static const oh_output_point_t* bhLayout[bhLayoutSize] = BH_LAYOUT_TACTAL;

    auto body = new HapticBody();

    TestActuator* actuator0 = new TestActuator();
    TestActuator* actuator1 = new TestActuator();
    TestActuator* actuator2 = new TestActuator();
    TestActuator* actuator3 = new TestActuator();
    TestActuator* actuator4 = new TestActuator();
    TestActuator* actuator5 = new TestActuator();

    auto outputs = PlaneMapper_Margin::mapMatrixCoordinates<AbstractActuator>({
      { actuator0, actuator1, actuator2, actuator3, actuator4, actuator5 },
    });
    auto plane = new HapticPlane(outputs);

    body->addComponent(OUTPUT_PATH_ACCESSORY, plane);
    body->setup();

    uint8_t values[] = { 0x64, 0x00, 0x00, 0x00, 0x00, 0x00 };
    std::string value = std::string((char*) values, sizeof(values));

    plainOutputTransformer(body, value, bhLayout, bhLayoutSize, OUTPUT_PATH_ACCESSORY);
    TEST_ASSERT_EQUAL_INT(4095, actuator0->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuator1->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuator2->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuator3->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuator4->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuator5->intensity);

    value = "\x10\x20\x30\x40\x50\x60";

    plainOutputTransformer(body, value, bhLayout, bhLayoutSize, OUTPUT_PATH_ACCESSORY);
    TEST_ASSERT_EQUAL_INT(655, actuator0->intensity);
    TEST_ASSERT_EQUAL_INT(1310, actuator1->intensity);
    TEST_ASSERT_EQUAL_INT(1965, actuator2->intensity);
    TEST_ASSERT_EQUAL_INT(2620, actuator3->intensity);
    TEST_ASSERT_EQUAL_INT(3276, actuator4->intensity);
    TEST_ASSERT_EQUAL_INT(3931, actuator5->intensity);
}

void test_layout_tactosy2(void)
{
    static const size_t bhLayoutSize = BH_LAYOUT_TACTOSY2_SIZE;
    static const oh_output_point_t* bhLayout[bhLayoutSize] = BH_LAYOUT_TACTOSY2;

    auto body = new HapticBody();

    TestActuator* actuator0 = new TestActuator();
    TestActuator* actuator1 = new TestActuator();
    TestActuator* actuator2 = new TestActuator();
    TestActuator* actuator3 = new TestActuator();
    TestActuator* actuator4 = new TestActuator();
    TestActuator* actuator5 = new TestActuator();

    auto outputs = PlaneMapper_Margin::mapMatrixCoordinates<AbstractActuator>({
      { actuator0, actuator1, actuator2 },
      { actuator3, actuator4, actuator5 },
    });
    auto plane = new HapticPlane(outputs);

    body->addComponent(OUTPUT_PATH_ACCESSORY, plane);
    body->setup();

    uint8_t values[] = { 0x64, 0x00, 0x00, 0x00, 0x00, 0x00 };
    std::string value = std::string((char*) values, sizeof(values));

    plainOutputTransformer(body, value, bhLayout, bhLayoutSize, OUTPUT_PATH_ACCESSORY);
    TEST_ASSERT_EQUAL_INT(4095, actuator0->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuator1->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuator2->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuator3->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuator4->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuator5->intensity);

    value = "\x10\x20\x30\x40\x50\x60";
    plainOutputTransformer(body, value, bhLayout, bhLayoutSize, OUTPUT_PATH_ACCESSORY);
    TEST_ASSERT_EQUAL_INT(655, actuator0->intensity);
    TEST_ASSERT_EQUAL_INT(1310, actuator1->intensity);
    TEST_ASSERT_EQUAL_INT(1965, actuator2->intensity);
    TEST_ASSERT_EQUAL_INT(2620, actuator3->intensity);
    TEST_ASSERT_EQUAL_INT(3276, actuator4->intensity);
    TEST_ASSERT_EQUAL_INT(3931, actuator5->intensity);
}

void test_layout_tactosyh(void)
{
    static const size_t bhLayoutSize = BH_LAYOUT_TACTOSYH_SIZE;
    static const oh_output_point_t* bhLayout[bhLayoutSize] = BH_LAYOUT_TACTOSYH;

    auto body = new HapticBody();

    TestActuator* actuator0 = new TestActuator();
    TestActuator* actuator1 = new TestActuator();
    TestActuator* actuator2 = new TestActuator();

    auto outputs = PlaneMapper_Margin::mapMatrixCoordinates<AbstractActuator>({
      { actuator0 },
      { actuator1 },
      { actuator2 },
    });
    auto plane = new HapticPlane(outputs);

    body->addComponent(OUTPUT_PATH_ACCESSORY, plane);
    body->setup();

    uint8_t values[] = { 0x64, 0x00, 0x00 };
    std::string value = std::string((char*) values, sizeof(values));

    plainOutputTransformer(body, value, bhLayout, bhLayoutSize, OUTPUT_PATH_ACCESSORY);
    TEST_ASSERT_EQUAL_INT(4095, actuator0->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuator1->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuator2->intensity);

    value = "\x10\x20\x30";
    plainOutputTransformer(body, value, bhLayout, bhLayoutSize, OUTPUT_PATH_ACCESSORY);
    TEST_ASSERT_EQUAL_INT(655, actuator0->intensity);
    TEST_ASSERT_EQUAL_INT(1310, actuator1->intensity);
    TEST_ASSERT_EQUAL_INT(1965, actuator2->intensity);
}

void test_layout_tactosyf(void)
{
    static const size_t bhLayoutSize = BH_LAYOUT_TACTOSYF_SIZE;
    static const oh_output_point_t* bhLayout[bhLayoutSize] = BH_LAYOUT_TACTOSYF;

    auto body = new HapticBody();

    TestActuator* actuator0 = new TestActuator();
    TestActuator* actuator1 = new TestActuator();
    TestActuator* actuator2 = new TestActuator();

    auto outputs = PlaneMapper_Margin::mapMatrixCoordinates<AbstractActuator>({
      { actuator0 },
      { actuator1 },
      { actuator2 },
    });
    auto plane = new HapticPlane(outputs);

    body->addComponent(OUTPUT_PATH_ACCESSORY, plane);
    body->setup();

    uint8_t values[] = { 0x64, 0x00, 0x00 };
    std::string value = std::string((char*) values, sizeof(values));

    plainOutputTransformer(body, value, bhLayout, bhLayoutSize, OUTPUT_PATH_ACCESSORY);
    TEST_ASSERT_EQUAL_INT(4095, actuator0->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuator1->intensity);
    TEST_ASSERT_EQUAL_INT(0, actuator2->intensity);

    value = "\x10\x20\x30";
    plainOutputTransformer(body, value, bhLayout, bhLayoutSize, OUTPUT_PATH_ACCESSORY);
    TEST_ASSERT_EQUAL_INT(655, actuator0->intensity);
    TEST_ASSERT_EQUAL_INT(1310, actuator1->intensity);
    TEST_ASSERT_EQUAL_INT(1965, actuator2->intensity);
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_layout_tactsuitx16);
    RUN_TEST(test_layout_tactsuitx40);
    RUN_TEST(test_layout_tactal);
    RUN_TEST(test_layout_tactosy2);
    RUN_TEST(test_layout_tactosyh);
    RUN_TEST(test_layout_tactosyf);

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
