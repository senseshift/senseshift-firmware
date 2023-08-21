#include <senseshift/math/point2.hpp>
#include <unity.h>

using namespace SenseShift::Math;

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

void test_operator_minus(void)
{
    Point2 p1 = { 32, 32 };
    Point2 p2 = { 16, 16 };

    TEST_ASSERT_EQUAL_FLOAT(p1 - p2, p2 - p1);
    TEST_ASSERT_EQUAL_FLOAT(22.6274, p1 - p2);
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_operator_equal);
    RUN_TEST(test_operator_not_equal);
    RUN_TEST(test_operator_less_than);
    RUN_TEST(test_operator_minus);

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
