#include <unity.h>

#include <haptic_body.hpp>

using namespace SenseShift::Body::Haptics;

class TestActuator : public OH::AbstractActuator {
  public:
    bool isSetup = false;
    Plane::Intensity_t intensity = 0;

    TestActuator() : AbstractActuator() {}
    void setup() override
    {
        this->isSetup = true;
    }
    void writeOutput(Plane::Intensity_t intensity) override
    {
        this->intensity = intensity;
    }
};

void test_it_sets_up_planes(void)
{
    auto body = new HapticBody();
    auto actuator = new TestActuator(), actuator2 = new TestActuator(), actuator3 = new TestActuator(),
         actuator4 = new TestActuator();

    Plane::ActuatorMap_t outputs = {
        { { 0, 0 }, actuator },
        { { 0, 255 }, actuator2 },
        { { 255, 0 }, actuator3 },
        { { 255, 255 }, actuator4 },
    };
    auto plane = new Plane(outputs);

    body->addComponent(HapticBody::Target_t::ChestFront, HapticBody::Layer_t::Vibro, plane);
    body->setup();

    for (auto& kv : outputs) {
        TEST_ASSERT_TRUE(static_cast<TestActuator*>(kv.second)->isSetup);
    }
}

void test_it_handles_message(void)
{
    auto body = new HapticBody();
    auto actuator = new TestActuator(), actuator2 = new TestActuator(), actuator3 = new TestActuator(),
         actuator4 = new TestActuator();

    Plane::ActuatorMap_t outputs = {
        { { 0, 0 }, actuator },
        { { 0, 255 }, actuator2 },
        { { 255, 0 }, actuator3 },
        { { 255, 255 }, actuator4 },
    };
    auto plane = new Plane(outputs);

    body->addComponent(HapticBody::Target_t::ChestFront, HapticBody::Layer_t::Vibro, plane);

    body->handleMessage({ HapticBody::Target_t::ChestFront, HapticBody::Layer_t::Vibro, { 0, 0 }, 64 });
    body->handleMessage({ HapticBody::Target_t::ChestFront, HapticBody::Layer_t::Vibro, { 0, 255 }, 128 });
    body->handleMessage({ HapticBody::Target_t::ChestFront, HapticBody::Layer_t::Vibro, { 255, 0 }, 192 });
    body->handleMessage({ HapticBody::Target_t::ChestFront, HapticBody::Layer_t::Vibro, { 255, 255 }, 255 });

    TEST_ASSERT_EQUAL_UINT8(64, actuator->intensity);
    TEST_ASSERT_EQUAL_UINT8(128, actuator2->intensity);
    TEST_ASSERT_EQUAL_UINT8(192, actuator3->intensity);
    TEST_ASSERT_EQUAL_UINT8(255, actuator4->intensity);
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_it_sets_up_planes);
    RUN_TEST(test_it_handles_message);

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
