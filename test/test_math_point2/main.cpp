#include <point2.hpp>
#include <unity.h>

using namespace OH;

void test_operator_equal(void)
{
    Point2 p1 = { 1, 2 };
    Point2 p2 = { 1, 2 };
    Point2 p3 = { 2, 1 };

    TEST_ASSERT_TRUE(p1 == p2);
    TEST_ASSERT_FALSE(p1 == p3);
}

void test_operator_not_equal(void)
{
    Point2 p1 = { 1, 2 };
    Point2 p2 = { 1, 2 };
    Point2 p3 = { 2, 1 };

    TEST_ASSERT_FALSE(p1 != p2);
    TEST_ASSERT_TRUE(p1 != p3);
}

void test_operator_less_than(void)
{
    Point2 p1 = { 1, 2 };
    Point2 p2 = { 1, 2 };
    Point2 p3 = { 2, 1 };

    TEST_ASSERT_FALSE(p1 < p2);
    TEST_ASSERT_TRUE(p1 < p3);
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_operator_equal);
    RUN_TEST(test_operator_not_equal);
    RUN_TEST(test_operator_less_than);

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
