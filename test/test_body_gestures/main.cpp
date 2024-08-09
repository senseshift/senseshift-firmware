#include <senseshift/body/hands/input/gesture.hpp>
#include <senseshift/input/sensor.hpp>
#include <unity.h>

using namespace SenseShift::Input;
using namespace SenseShift::Body::Hands::Input;

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

void test_gesture_trigger(void)
{
    float threshold = 0.5f;
    auto* index = new FloatSensor();

    auto* gesture = new TriggerGesture(index, threshold, true);
    gesture->init();

    index->publishState(0.4f);
    TEST_ASSERT_FALSE(gesture->getValue());

    index->publishState(0.6f);
    TEST_ASSERT_TRUE(gesture->getValue());
}

void test_gesture_grab(void)
{
    float threshold = 0.5f;

    auto* index = new FloatSensor();
    auto* middle = new FloatSensor();
    auto* ring = new FloatSensor();
    auto* pinky = new FloatSensor();

    auto* gesture = new GrabGesture(
      {
        .index = index,
        .middle = middle,
        .ring = ring,
        .pinky = pinky,
      },
      threshold,
      true
    );
    gesture->init();

    TEST_ASSERT_FALSE(gesture->getValue());

    index->publishState(0.6);
    TEST_ASSERT_FALSE(gesture->getValue());

    middle->publishState(0.6);
    TEST_ASSERT_FALSE(gesture->getValue());

    ring->publishState(0.6);
    TEST_ASSERT_FALSE(gesture->getValue());

    pinky->publishState(0.6);
    TEST_ASSERT_TRUE(gesture->getValue());
}

void test_gesture_pinch(void)
{
    float threshold = 0.5f;

    auto* thumb = new FloatSensor();
    auto* index = new FloatSensor();

    auto* gesture = new PinchGesture({ .thumb = thumb, .index = index }, threshold, true);
    gesture->init();

    TEST_ASSERT_FALSE(gesture->getValue());

    thumb->publishState(0.6f);
    TEST_ASSERT_FALSE(gesture->getValue());

    index->publishState(0.6f);
    TEST_ASSERT_TRUE(gesture->getValue());
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_gesture_grab);
    RUN_TEST(test_gesture_trigger);
    RUN_TEST(test_gesture_pinch);

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
