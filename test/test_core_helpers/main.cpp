#include <senseshift/core/helpers.hpp>
#include <unity.h>

#include <map>
#include <cstdint>

using namespace SenseShift;

void test_lerp_uint8(void)
{
    TEST_ASSERT_EQUAL_UINT16(0, lerp<std::uint8_t>(0.0f, 0, 255));
    TEST_ASSERT_EQUAL_UINT16(127, lerp<std::uint8_t>(0.5f, 0, 255));
    TEST_ASSERT_EQUAL_UINT16(255, lerp<std::uint8_t>(1.0f, 0, 255));

    TEST_ASSERT_EQUAL_UINT16(0, lerp<std::uint8_t>(0.0f, 0, 0));
}

void test_lerp_float(void)
{
    TEST_ASSERT_EQUAL_FLOAT(-1.0f, lerp<float>(0.0f, -1.0f, 1.0f));
    TEST_ASSERT_EQUAL_FLOAT(0.0f, lerp<float>(0.5f, -1.0f, 1.0f));
    TEST_ASSERT_EQUAL_FLOAT(1.0f, lerp<float>(1.0f, -1.0f, 1.0f));

    TEST_ASSERT_EQUAL_FLOAT(0.0f, lerp<float>(0.0f, 0.0f, 0.0f));
}

void test_remap_uint16(void)
{
    TEST_ASSERT_EQUAL_UINT16(0, remap(0, 0, 4095, 0, 255));
    TEST_ASSERT_EQUAL_UINT16(127, remap(2047, 0, 4095, 0, 255));
    TEST_ASSERT_EQUAL_UINT16(255, remap(4095, 0, 4095, 0, 255));

    TEST_ASSERT_EQUAL_UINT16(127, remap(0, 0, 4095, 127, 255));
    TEST_ASSERT_EQUAL_UINT16(190, remap(2047, 0, 4095, 127, 255));
    TEST_ASSERT_EQUAL_UINT16(255, remap(4095, 0, 4095, 127, 255));

    TEST_ASSERT_EQUAL_UINT16(0, remap(2048, 2048, 4095, 0, 255));
    TEST_ASSERT_EQUAL_UINT16(127, remap(3071, 2048, 4095, 0, 255));
    TEST_ASSERT_EQUAL_UINT16(255, remap(4095, 2048, 4095, 0, 255));

    TEST_ASSERT_EQUAL_UINT16(2047, remap(343, 343, 343, 0, 4095));
}

void test_remap_float(void)
{
    TEST_ASSERT_EQUAL_FLOAT(0.0f, remap(0.0f, 0.0f, 1.0f, 0.0f, 255.0f));
    TEST_ASSERT_EQUAL_FLOAT(127.5f, remap(0.5f, 0.0f, 1.0f, 0.0f, 255.0f));
    TEST_ASSERT_EQUAL_FLOAT(255.0f, remap(1.0f, 0.0f, 1.0f, 0.0f, 255.0f));

    TEST_ASSERT_EQUAL_FLOAT(0.5f, remap(0.0f, -1.0f, 1.0f, 0.0f, 1.0f));

    TEST_ASSERT_EQUAL_FLOAT(0.125f, remap(0.2f, 0.1f, 0.9f, 0.f, 1.f));
}

void test_remap_simple_uint16(void)
{
    TEST_ASSERT_EQUAL_UINT16(0, remap_simple(0, 4095, 255));
    TEST_ASSERT_EQUAL_UINT16(127, remap_simple(2047, 4095, 255));
    TEST_ASSERT_EQUAL_UINT16(255, remap_simple(4095, 4095, 255));
}

void test_remap_simple_float(void)
{
    TEST_ASSERT_EQUAL_FLOAT(0.0f, remap_simple(0.0f, 1.0f, 255.0f));
    TEST_ASSERT_EQUAL_FLOAT(127.5f, remap_simple(0.5f, 1.0f, 255.0f));
    TEST_ASSERT_EQUAL_FLOAT(255.0f, remap_simple(1.0f, 1.0f, 255.0f));
}

// Wtf? https://stackoverflow.com/questions/4295890
#define COMMAE ,

void test_lookup_table_interpolate_float(void)
{
    const std::map<float, float> table = {
        { 0.0f, 13.0f },
        { 0.5f, 16.0f },
        { 0.6f, 17.0f },
        { 1.0f, 18.0f },
    };

    TEST_ASSERT_EQUAL_FLOAT(13.0f, lookup_table_interpolate<float COMMAE float>(table, 0.0f));
    TEST_ASSERT_EQUAL_FLOAT(13.6f, lookup_table_interpolate<float COMMAE float>(table, 0.1f));
    TEST_ASSERT_EQUAL_FLOAT(14.2f, lookup_table_interpolate<float COMMAE float>(table, 0.2f));
    TEST_ASSERT_EQUAL_FLOAT(14.5f, lookup_table_interpolate<float COMMAE float>(table, 0.25f));
    TEST_ASSERT_EQUAL_FLOAT(14.8f, lookup_table_interpolate<float COMMAE float>(table, 0.3f));
    TEST_ASSERT_EQUAL_FLOAT(15.4f, lookup_table_interpolate<float COMMAE float>(table, 0.4f));
    TEST_ASSERT_EQUAL_FLOAT(16.0f, lookup_table_interpolate<float COMMAE float>(table, 0.5f));
    TEST_ASSERT_EQUAL_FLOAT(16.5f, lookup_table_interpolate<float COMMAE float>(table, 0.55f));
    TEST_ASSERT_EQUAL_FLOAT(17.0f, lookup_table_interpolate<float COMMAE float>(table, 0.6f));
    TEST_ASSERT_EQUAL_FLOAT(17.25f, lookup_table_interpolate<float COMMAE float>(table, 0.7f));
    TEST_ASSERT_EQUAL_FLOAT(17.5f, lookup_table_interpolate<float COMMAE float>(table, 0.8f));
    TEST_ASSERT_EQUAL_FLOAT(17.75f, lookup_table_interpolate<float COMMAE float>(table, 0.9f));
    TEST_ASSERT_EQUAL_FLOAT(18.0f, lookup_table_interpolate<float COMMAE float>(table, 1.0f));
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_lerp_uint8);
    RUN_TEST(test_lerp_float);

    RUN_TEST(test_remap_uint16);
    RUN_TEST(test_remap_float);

    RUN_TEST(test_remap_simple_uint16);
    RUN_TEST(test_remap_simple_float);

    RUN_TEST(test_lookup_table_interpolate_float);

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
