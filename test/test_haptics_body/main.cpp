#include <haptic_body.hpp>
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

void test_it_sets_up_planes(void)
{
    auto body = new HapticBody();

    VibroPlane::ActuatorMap_t outputs = {
        { { 0, 0 }, new TestActuator() },
        { { 0, 1 }, new TestActuator() },
        { { 1, 0 }, new TestActuator() },
        { { 1, 1 }, new TestActuator() },
    };
    auto plane = new VibroPlane(outputs);

    body->addTarget(Target::ChestFront, plane);
    body->setup();

    for (auto& kv : outputs) {
        TEST_ASSERT_TRUE_MESSAGE(static_cast<TestActuator*>(kv.second)->isSetup, "Actuator was not setup");
    }
}

void test_it_handles_effect__vibro(void)
{
    auto actuator1 = new TestActuator(), actuator2 = new TestActuator(), actuator3 = new TestActuator(),
         actuator4 = new TestActuator();

    auto body = new HapticBody();

    auto plane = new VibroPlane({
      { { 0, 0 }, actuator1 },
      { { 0, 1 }, actuator2 },
      { { 1, 0 }, actuator3 },
      { { 1, 1 }, actuator4 },
    });

    body->addTarget(Target::ChestFront, plane);

    body->effect({
      .effect = Effect::Vibro,
      .target = Target::ChestFront,
      .position = { 0, 0 },
      .data = (VibroEffectData_t) 64,
    });
    body->effect({
      .effect = Effect::Vibro,
      .target = Target::ChestFront,
      .position = { 0, 1 },
      .data = (VibroEffectData_t) 128,
    });
    body->effect({
      .effect = Effect::Vibro,
      .target = Target::ChestFront,
      .position = { 1, 0 },
      .data = (VibroEffectData_t) 192,
    });
    body->effect({
      .effect = Effect::Vibro,
      .target = Target::ChestFront,
      .position = { 1, 1 },
      .data = (VibroEffectData_t) 255,
    });

    TEST_ASSERT_EQUAL(64, actuator1->intensity);
    TEST_ASSERT_EQUAL(128, actuator2->intensity);
    TEST_ASSERT_EQUAL(192, actuator3->intensity);
    TEST_ASSERT_EQUAL(255, actuator4->intensity);
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_it_sets_up_planes);
    RUN_TEST(test_it_handles_effect__vibro);

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
