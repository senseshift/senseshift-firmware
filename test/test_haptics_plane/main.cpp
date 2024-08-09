#include <senseshift/body/haptics/plane.hpp>
#include <unity.h>

using namespace SenseShift::Body::Haptics;
using namespace SenseShift::Output;

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

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

void test_it_sets_up_actuators(void)
{
    FloatPlane::ActuatorMap outputs = {
        { { 0, 0 }, new TestActuator() },
        { { 0, 1 }, new TestActuator() },
        { { 1, 0 }, new TestActuator() },
        { { 1, 1 }, new TestActuator() },
    };

    auto plane = new FloatPlane(outputs);
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

    FloatPlane::ActuatorMap outputs = {
        { { 0, 0 }, actuator },
        { { 0, 1 }, actuator2 },
        { { 1, 0 }, actuator3 },
        { { 1, 1 }, actuator4 },
    };

    auto plane = new FloatPlane(outputs);

    plane->effect({ 0, 0 }, 0.25F);
    plane->effect({ 0, 1 }, 0.5F);
    plane->effect({ 1, 0 }, 0.75F);
    plane->effect({ 1, 1 }, 1.0F);

    TEST_ASSERT_EQUAL_FLOAT(0.25F, actuator->intensity);
    TEST_ASSERT_EQUAL_FLOAT(0.5F, actuator2->intensity);
    TEST_ASSERT_EQUAL_FLOAT(0.75F, actuator3->intensity);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, actuator4->intensity);
}

void test_it_updates_state(void)
{
    auto actuator = new TestActuator(), actuator2 = new TestActuator(), actuator3 = new TestActuator(),
         actuator4 = new TestActuator();

    FloatPlane::ActuatorMap outputs = {
        { { 0, 0 }, actuator },
        { { 0, 1 }, actuator2 },
        { { 1, 0 }, actuator3 },
        { { 1, 1 }, actuator4 },
    };

    auto plane = new FloatPlane(outputs);

    plane->effect({ 0, 0 }, 0.25F);
    plane->effect({ 0, 1 }, 0.5F);
    plane->effect({ 1, 0 }, 0.75F);
    plane->effect({ 1, 1 }, 1.0F);

    TEST_ASSERT_TRUE(plane->getActuatorStates()->count({ 0, 0 }) > 0);
    TEST_ASSERT_EQUAL_FLOAT(0.25F, plane->getActuatorStates()->at({ 0, 0 }));

    TEST_ASSERT_TRUE(plane->getActuatorStates()->count({ 0, 1 }) > 0);
    TEST_ASSERT_EQUAL_FLOAT(0.5F, plane->getActuatorStates()->at({ 0, 1 }));

    TEST_ASSERT_TRUE(plane->getActuatorStates()->count({ 1, 0 }) > 0);
    TEST_ASSERT_EQUAL_FLOAT(0.75F, plane->getActuatorStates()->at({ 1, 0 }));

    TEST_ASSERT_TRUE(plane->getActuatorStates()->count({ 1, 1 }) > 0);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, plane->getActuatorStates()->at({ 1, 1 }));
}

void test_closest_it_writes_to_correct_if_exact(void)
{
    auto actuator = new TestActuator(), actuator2 = new TestActuator(), actuator3 = new TestActuator(),
         actuator4 = new TestActuator();

    FloatPlane_Closest::ActuatorMap outputs = {
        { { 0, 0 }, actuator },
        { { 0, 1 }, actuator2 },
        { { 1, 0 }, actuator3 },
        { { 1, 1 }, actuator4 },
    };

    auto plane = new FloatPlane_Closest(outputs);

    plane->effect({ 0, 0 }, 0.25F);
    plane->effect({ 0, 1 }, 0.5F);
    plane->effect({ 1, 0 }, 0.75F);
    plane->effect({ 1, 1 }, 1.0F);

    TEST_ASSERT_EQUAL_FLOAT(0.25F, actuator->intensity);
    TEST_ASSERT_EQUAL_FLOAT(0.5F, actuator2->intensity);
    TEST_ASSERT_EQUAL_FLOAT(0.75F, actuator3->intensity);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, actuator4->intensity);
}

void test_closest_it_correctly_finds_closest(void)
{
    auto actuator = new TestActuator(), actuator2 = new TestActuator(), actuator3 = new TestActuator(),
         actuator4 = new TestActuator();

    FloatPlane_Closest::ActuatorMap outputs = {
        { { 0, 0 }, actuator },
        { { 0, 64 }, actuator2 },
        { { 64, 0 }, actuator3 },
        { { 64, 64 }, actuator4 },
    };

    auto plane = new FloatPlane_Closest(outputs);

    plane->effect({ 16, 16 }, 0.25F);
    plane->effect({ 65, 65 }, 0.5F);

    TEST_ASSERT_EQUAL_FLOAT(0.25F, actuator->intensity);
    TEST_ASSERT_EQUAL_FLOAT(0, actuator2->intensity);
    TEST_ASSERT_EQUAL_FLOAT(0, actuator3->intensity);
    TEST_ASSERT_EQUAL_FLOAT(0.5F, actuator4->intensity);
}

void test_closest_it_updates_state(void)
{
    auto actuator = new TestActuator(), actuator2 = new TestActuator(), actuator3 = new TestActuator(),
         actuator4 = new TestActuator();

    FloatPlane_Closest::ActuatorMap outputs = {
        { { 0, 0 }, actuator },
        { { 0, 64 }, actuator2 },
        { { 64, 0 }, actuator3 },
        { { 64, 64 }, actuator4 },
    };

    auto plane = new FloatPlane_Closest(outputs);

    plane->effect({ 16, 16 }, 0.25F);
    plane->effect({ 65, 65 }, 0.5F);

    TEST_ASSERT_TRUE(plane->getActuatorStates()->count({ 0, 0 }) > 0);
    TEST_ASSERT_EQUAL_FLOAT(0.25F, plane->getActuatorStates()->at({ 0, 0 }));

    TEST_ASSERT_TRUE(plane->getActuatorStates()->count({ 0, 64 }) > 0);
    TEST_ASSERT_EQUAL_FLOAT(0, plane->getActuatorStates()->at({ 0, 64 }));

    TEST_ASSERT_TRUE(plane->getActuatorStates()->count({ 64, 0 }) > 0);
    TEST_ASSERT_EQUAL_FLOAT(0, plane->getActuatorStates()->at({ 64, 0 }));

    TEST_ASSERT_TRUE(plane->getActuatorStates()->count({ 64, 64 }) > 0);
    TEST_ASSERT_EQUAL_FLOAT(0.5F, plane->getActuatorStates()->at({ 64, 64 }));
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

void loop(void)
{
}

#else

int main()
{
    return process();
}

#endif
