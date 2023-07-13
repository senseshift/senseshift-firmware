#include <sensor/og_gesture.hpp>
#include <unity.h>

using namespace OpenGloves;

class TestCurlFinger : public ICurl {
  public:
    uint16_t value;

    TestCurlFinger(uint16_t initialValue = 0) : value(initialValue){};

    uint16_t getCurl() override
    {
        return this->value;
    }
};

void test_gesture_grab(void)
{
    uint16_t threshold = 2047;

    auto* index = new TestCurlFinger(threshold - 1);
    auto* middle = new TestCurlFinger(threshold - 1);
    auto* ring = new TestCurlFinger(threshold - 1);
    auto* pinky = new TestCurlFinger(threshold - 1);

    auto* gesture = new GrabGesture(index, middle, ring, pinky, threshold);

    TEST_ASSERT_FALSE(gesture->getValue());

    index->value = threshold + 1;
    TEST_ASSERT_FALSE(gesture->getValue());

    middle->value = threshold + 1;
    TEST_ASSERT_FALSE(gesture->getValue());

    ring->value = threshold + 1;
    TEST_ASSERT_FALSE(gesture->getValue());

    pinky->value = threshold + 1;
    TEST_ASSERT_TRUE(gesture->getValue());
}

void test_gesture_trigger(void)
{
    uint16_t threshold = 2047;

    auto* index = new TestCurlFinger(threshold - 1);

    auto* gesture = new TriggerGesture(index, threshold);

    TEST_ASSERT_FALSE(gesture->getValue());

    index->value = threshold + 1;
    TEST_ASSERT_TRUE(gesture->getValue());
}

void test_gesture_pinch(void)
{
    uint16_t threshold = 2047;

    auto* index = new TestCurlFinger(threshold - 1);
    auto* middle = new TestCurlFinger(threshold - 1);

    auto* gesture = new PinchGesture(index, middle, threshold);

    TEST_ASSERT_FALSE(gesture->getValue());

    index->value = threshold + 1;
    TEST_ASSERT_FALSE(gesture->getValue());

    middle->value = threshold + 1;
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

void loop(void) {}

#else

int main(int argc, char** argv)
{
    return process();
}

#endif
