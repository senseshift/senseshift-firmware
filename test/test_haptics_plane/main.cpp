#include <senseshift/body/haptics/plane.hpp>
#include <unity.h>

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

void test_it_sets_up_actuators(void)
{
    VibroPlane::ActuatorMap_t outputs = {
        { { 0, 0 }, new TestActuator() },
        { { 0, 1 }, new TestActuator() },
        { { 1, 0 }, new TestActuator() },
        { { 1, 1 }, new TestActuator() },
    };

    auto plane = new VibroPlane(outputs);
    plane->setup();

    TEST_ASSERT_EQUAL(outputs.size(), plane->getAvailablePoints()->size());
    for (auto& kv : outputs) {
        TEST_ASSERT_TRUE_MESSAGE(plane->getAvailablePoints()->count(kv.first) > 0, "Expected point was not found");
        TEST_ASSERT_TRUE_MESSAGE(plane->getActuatorStates()->count(kv.first) > 0, "Expected state was not found");
        TEST_ASSERT_TRUE_MESSAGE(static_cast<TestActuator*>(kv.second)->isSetup, "Actuator was not setup");
    }
}

void test_it_writes_to_correct_output(void)
{
    auto actuator = new TestActuator(), actuator2 = new TestActuator(), actuator3 = new TestActuator(),
         actuator4 = new TestActuator();

    VibroPlane::ActuatorMap_t outputs = {
        { { 0, 0 }, actuator },
        { { 0, 1 }, actuator2 },
        { { 1, 0 }, actuator3 },
        { { 1, 1 }, actuator4 },
    };

    auto plane = new VibroPlane(outputs);

    plane->effect({ 0, 0 }, 64);
    plane->effect({ 0, 1 }, 128);
    plane->effect({ 1, 0 }, 192);
    plane->effect({ 1, 1 }, 255);

    TEST_ASSERT_EQUAL_UINT8(64, actuator->intensity);
    TEST_ASSERT_EQUAL_UINT8(128, actuator2->intensity);
    TEST_ASSERT_EQUAL_UINT8(192, actuator3->intensity);
    TEST_ASSERT_EQUAL_UINT8(255, actuator4->intensity);
}

void test_it_updates_state(void)
{
    auto actuator = new TestActuator(), actuator2 = new TestActuator(), actuator3 = new TestActuator(),
         actuator4 = new TestActuator();

    VibroPlane::ActuatorMap_t outputs = {
        { { 0, 0 }, actuator },
        { { 0, 1 }, actuator2 },
        { { 1, 0 }, actuator3 },
        { { 1, 1 }, actuator4 },
    };

    auto plane = new VibroPlane(outputs);

    plane->effect({ 0, 0 }, 64);
    plane->effect({ 0, 1 }, 128);
    plane->effect({ 1, 0 }, 192);
    plane->effect({ 1, 1 }, 255);

    TEST_ASSERT_TRUE(plane->getActuatorStates()->count({ 0, 0 }) > 0);
    TEST_ASSERT_EQUAL_UINT8(64, plane->getActuatorStates()->at({ 0, 0 }));

    TEST_ASSERT_TRUE(plane->getActuatorStates()->count({ 0, 1 }) > 0);
    TEST_ASSERT_EQUAL_UINT8(128, plane->getActuatorStates()->at({ 0, 1 }));

    TEST_ASSERT_TRUE(plane->getActuatorStates()->count({ 1, 0 }) > 0);
    TEST_ASSERT_EQUAL_UINT8(192, plane->getActuatorStates()->at({ 1, 0 }));

    TEST_ASSERT_TRUE(plane->getActuatorStates()->count({ 1, 1 }) > 0);
    TEST_ASSERT_EQUAL_UINT8(255, plane->getActuatorStates()->at({ 1, 1 }));
}

void test_closest_it_writes_to_correct_if_exact(void)
{
    auto actuator = new TestActuator(), actuator2 = new TestActuator(), actuator3 = new TestActuator(),
         actuator4 = new TestActuator();

    VibroPlane_Closest::ActuatorMap_t outputs = {
        { { 0, 0 }, actuator },
        { { 0, 1 }, actuator2 },
        { { 1, 0 }, actuator3 },
        { { 1, 1 }, actuator4 },
    };

    auto plane = new VibroPlane_Closest(outputs);

    plane->effect({ 0, 0 }, 1);
    plane->effect({ 0, 1 }, 2);
    plane->effect({ 1, 0 }, 3);
    plane->effect({ 1, 1 }, 4);

    TEST_ASSERT_EQUAL(1, actuator->intensity);
    TEST_ASSERT_EQUAL(2, actuator2->intensity);
    TEST_ASSERT_EQUAL(3, actuator3->intensity);
    TEST_ASSERT_EQUAL(4, actuator4->intensity);
}

void test_closest_it_correctly_finds_closest(void)
{
    auto actuator = new TestActuator(), actuator2 = new TestActuator(), actuator3 = new TestActuator(),
         actuator4 = new TestActuator();

    VibroPlane_Closest::ActuatorMap_t outputs = {
        { { 0, 0 }, actuator },
        { { 0, 64 }, actuator2 },
        { { 64, 0 }, actuator3 },
        { { 64, 64 }, actuator4 },
    };

    auto plane = new VibroPlane_Closest(outputs);

    plane->effect({ 16, 16 }, 16);
    plane->effect({ 65, 65 }, 65);

    TEST_ASSERT_EQUAL(16, actuator->intensity);
    TEST_ASSERT_EQUAL(0, actuator2->intensity);
    TEST_ASSERT_EQUAL(0, actuator3->intensity);
    TEST_ASSERT_EQUAL(65, actuator4->intensity);
}

void test_closest_it_updates_state(void)
{
    auto actuator = new TestActuator(), actuator2 = new TestActuator(), actuator3 = new TestActuator(),
         actuator4 = new TestActuator();

    VibroPlane_Closest::ActuatorMap_t outputs = {
        { { 0, 0 }, actuator },
        { { 0, 64 }, actuator2 },
        { { 64, 0 }, actuator3 },
        { { 64, 64 }, actuator4 },
    };

    auto plane = new VibroPlane_Closest(outputs);

    plane->effect({ 16, 16 }, 16);
    plane->effect({ 65, 65 }, 65);

    TEST_ASSERT_TRUE(plane->getActuatorStates()->count({ 0, 0 }) > 0);
    TEST_ASSERT_EQUAL(16, plane->getActuatorStates()->at({ 0, 0 }));

    TEST_ASSERT_TRUE(plane->getActuatorStates()->count({ 0, 64 }) > 0);
    TEST_ASSERT_EQUAL(0, plane->getActuatorStates()->at({ 0, 64 }));

    TEST_ASSERT_TRUE(plane->getActuatorStates()->count({ 64, 0 }) > 0);
    TEST_ASSERT_EQUAL(0, plane->getActuatorStates()->at({ 64, 0 }));

    TEST_ASSERT_TRUE(plane->getActuatorStates()->count({ 64, 64 }) > 0);
    TEST_ASSERT_EQUAL(65, plane->getActuatorStates()->at({ 64, 64 }));
}

void test_plain_mapper_margin_map_points(void)
{
    auto point = PlaneMapper_Margin::mapPoint<uint8_t>(0, 0, 0, 0);

    TEST_ASSERT_EQUAL(127, point.x);
    TEST_ASSERT_EQUAL(127, point.y);

    point = PlaneMapper_Margin::mapPoint<uint8_t>(0, 0, 1, 1);

    TEST_ASSERT_EQUAL(85, point.x);
    TEST_ASSERT_EQUAL(85, point.y);

    point = PlaneMapper_Margin::mapPoint<uint8_t>(1, 1, 1, 1);

    TEST_ASSERT_EQUAL(170, point.x);
    TEST_ASSERT_EQUAL(170, point.y);
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_it_sets_up_actuators);
    RUN_TEST(test_it_writes_to_correct_output);
    RUN_TEST(test_it_updates_state);

    RUN_TEST(test_closest_it_writes_to_correct_if_exact);
    RUN_TEST(test_closest_it_correctly_finds_closest);
    RUN_TEST(test_closest_it_updates_state);

    RUN_TEST(test_plain_mapper_margin_map_points);

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
