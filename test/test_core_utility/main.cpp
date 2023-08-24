#include <senseshift/utility.hpp>
#include <unity.h>

using namespace SenseShift;

void test_contains_container(void)
{
    std::vector<int> v = { 1, 2, 3, 4, 5 };
    TEST_ASSERT_TRUE(contains(v, 1));
    TEST_ASSERT_TRUE(contains(v, 2));
    TEST_ASSERT_TRUE(contains(v, 3));
    TEST_ASSERT_TRUE(contains(v, 4));
    TEST_ASSERT_TRUE(contains(v, 5));
    TEST_ASSERT_FALSE(contains(v, 6));
}

void test_contains_iterator(void)
{
    int arr[] = { 1, 2, 3, 4, 5 };
    TEST_ASSERT_TRUE(contains(arr, arr + 5, 1));
    TEST_ASSERT_TRUE(contains(arr, arr + 5, 2));
    TEST_ASSERT_TRUE(contains(arr, arr + 5, 3));
    TEST_ASSERT_TRUE(contains(arr, arr + 5, 4));
    TEST_ASSERT_TRUE(contains(arr, arr + 5, 5));
    TEST_ASSERT_FALSE(contains(arr, arr + 5, 6));
}

void test_contains_string(void)
{
    std::string s = "Hello, World!";
    TEST_ASSERT_TRUE(contains(s, 'H'));
    TEST_ASSERT_TRUE(contains(s, 'e'));
    TEST_ASSERT_TRUE(contains(s, 'l'));
    TEST_ASSERT_TRUE(contains(s, 'o'));
    TEST_ASSERT_TRUE(contains(s, ','));
    TEST_ASSERT_TRUE(contains(s, ' '));
    TEST_ASSERT_TRUE(contains(s, 'W'));
    TEST_ASSERT_TRUE(contains(s, 'r'));
    TEST_ASSERT_TRUE(contains(s, 'd'));
    TEST_ASSERT_TRUE(contains(s, '!'));
    TEST_ASSERT_FALSE(contains(s, 'h'));
    TEST_ASSERT_FALSE(contains(s, 'w'));
    TEST_ASSERT_FALSE(contains(s, 'x'));
    TEST_ASSERT_FALSE(contains(s, 'y'));
    TEST_ASSERT_FALSE(contains(s, 'z'));
}

void test_accurate_map(void)
{
    TEST_ASSERT_EQUAL_UINT16(0, accurateMap(0, 0, 4095, 0, 255));
    TEST_ASSERT_EQUAL_UINT16(127, accurateMap(2047, 0, 4095, 0, 255));
    TEST_ASSERT_EQUAL_UINT16(255, accurateMap(4095, 0, 4095, 0, 255));

    TEST_ASSERT_EQUAL_UINT16(127, accurateMap(0, 0, 4095, 127, 255));
    TEST_ASSERT_EQUAL_UINT16(190, accurateMap(2047, 0, 4095, 127, 255));
    TEST_ASSERT_EQUAL_UINT16(255, accurateMap(4095, 0, 4095, 127, 255));

    TEST_ASSERT_EQUAL_UINT16(0, accurateMap(2048, 2048, 4095, 0, 255));
    TEST_ASSERT_EQUAL_UINT16(127, accurateMap(3071, 2048, 4095, 0, 255));
    TEST_ASSERT_EQUAL_UINT16(255, accurateMap(4095, 2048, 4095, 0, 255));

    TEST_ASSERT_EQUAL_UINT16(2047, accurateMap(343, 343, 343, 0, 4095));
}

void test_simple_map(void)
{
    TEST_ASSERT_EQUAL_UINT16(0, simpleMap(0, 4095, 255));
    TEST_ASSERT_EQUAL_UINT16(127, simpleMap(2047, 4095, 255));
    TEST_ASSERT_EQUAL_UINT16(255, simpleMap(4095, 4095, 255));
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_contains_container);
    RUN_TEST(test_contains_iterator);
    RUN_TEST(test_contains_string);
    RUN_TEST(test_accurate_map);
    RUN_TEST(test_simple_map);

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
