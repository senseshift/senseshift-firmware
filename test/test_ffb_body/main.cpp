#include <unity.h>

#include <ffb_body.hpp>

using namespace SenseShift::Body::ForceFeedback;

class TestActuator : public OH::AbstractActuator {
  public:
    bool isSetup = false;
    FFBBody::Intensity_t intensity = 0;

    TestActuator() : AbstractActuator() {}
    void setup() override
    {
        this->isSetup = true;
    }
    void writeOutput(FFBBody::Intensity_t intensity) override
    {
        this->intensity = intensity;
    }
};

void test_it_sets_up_actuators(void)
{
    auto actuator = new TestActuator(), actuator2 = new TestActuator(), actuator3 = new TestActuator(),
         actuator4 = new TestActuator();

    FFBBody::ActuatorMap_t outputs = {
        { FFBBody::Joint_t::HandRightIndexMCP_Pitch, actuator },
        { FFBBody::Joint_t::HandRightIndexMCP_Yaw, actuator2 },
        { FFBBody::Joint_t::HandRightIndexPIP_Pitch, actuator3 },
        { FFBBody::Joint_t::HandRightIndexDIP_Pitch, actuator4 },
    };
    auto body = new FFBBody(outputs);

    body->setup();

    TEST_ASSERT_TRUE(actuator->isSetup);
    TEST_ASSERT_TRUE(actuator2->isSetup);
    TEST_ASSERT_TRUE(actuator3->isSetup);
    TEST_ASSERT_TRUE(actuator4->isSetup);
}

void test_it_handles_message(void)
{
    auto actuator = new TestActuator(), actuator2 = new TestActuator(), actuator3 = new TestActuator(),
         actuator4 = new TestActuator();

    FFBBody::ActuatorMap_t outputs = {
        { FFBBody::Joint_t::HandRightIndexMCP_Pitch, actuator },
        { FFBBody::Joint_t::HandRightIndexMCP_Yaw, actuator2 },
        { FFBBody::Joint_t::HandRightIndexPIP_Pitch, actuator3 },
        { FFBBody::Joint_t::HandRightIndexDIP_Pitch, actuator4 },
    };
    auto body = new FFBBody(outputs);

    body->handleMessage({ FFBBody::Joint_t::HandRightIndexMCP_Pitch, 64 });
    body->handleMessage({ FFBBody::Joint_t::HandRightIndexMCP_Yaw, 128 });
    body->handleMessage({ FFBBody::Joint_t::HandRightIndexPIP_Pitch, 192 });
    body->handleMessage({ FFBBody::Joint_t::HandRightIndexDIP_Pitch, 255 });

    TEST_ASSERT_EQUAL(64, actuator->intensity);
    TEST_ASSERT_EQUAL(128, actuator2->intensity);
    TEST_ASSERT_EQUAL(192, actuator3->intensity);
    TEST_ASSERT_EQUAL(255, actuator4->intensity);
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_it_sets_up_actuators);
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
