#include <senseshift/body/haptics/body.hpp>
#include <unity.h>

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

void test_it_sets_up_planes(void)
{
    auto body = new FloatBody();

    FloatPlane::ActuatorMap outputs = {
        { { 0, 0 }, new TestActuator() },
        { { 0, 1 }, new TestActuator() },
        { { 1, 0 }, new TestActuator() },
        { { 1, 1 }, new TestActuator() },
    };
    auto plane = new FloatPlane(outputs);

    body->addTarget(Target::ChestFront, plane);
    body->setup();

    for (auto& kv : outputs) {
        TEST_ASSERT_TRUE_MESSAGE(static_cast<TestActuator*>(kv.second)->isSetup, "Actuator was not setup");
    }
}

void test_it_handles_effect(void)
{
    auto actuator1 = new TestActuator(), actuator2 = new TestActuator(), actuator3 = new TestActuator(),
         actuator4 = new TestActuator();

    auto body = new FloatBody();

    auto plane = new FloatPlane({
      { { 0, 0 }, actuator1 },
      { { 0, 1 }, actuator2 },
      { { 1, 0 }, actuator3 },
      { { 1, 1 }, actuator4 },
    });

    body->addTarget(Target::ChestFront, plane);

    TEST_ASSERT_TRUE(body->getTarget(Target::ChestFront).has_value());

    body->effect(Target::ChestFront, { 0, 0 }, 0.25F);
    body->effect(Target::ChestFront, { 0, 1 }, 0.5F);
    body->effect(Target::ChestFront, { 1, 0 }, 0.75F);
    body->effect(Target::ChestFront, { 1, 1 }, 1.0F);

    TEST_ASSERT_EQUAL_FLOAT(0.25F, actuator1->intensity);
    TEST_ASSERT_EQUAL_FLOAT(0.5F, actuator2->intensity);
    TEST_ASSERT_EQUAL_FLOAT(0.75F, actuator3->intensity);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, actuator4->intensity);
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_it_sets_up_planes);
    RUN_TEST(test_it_handles_effect);

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
