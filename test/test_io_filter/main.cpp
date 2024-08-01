#include <senseshift/input/filter.hpp>
#include <senseshift/input/sensor/sensor.hpp>
#include <unity.h>

#include <map>

#define ASSERT_EQUAL_FLOAT_ROUNDED(expected, actual, precision)                \
    TEST_ASSERT_EQUAL_FLOAT(                                                   \
      std::round(expected* std::pow(10, precision)) / std::pow(10, precision), \
      std::round(actual * std::pow(10, precision)) / std::pow(10, precision)   \
    )

using namespace SenseShift::Input;
using namespace SenseShift::Input::Filter;

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

void test_add_filter(void)
{
    IFilter<float>* filter = new AddFilter<float>(5.0f);

    TEST_ASSERT_EQUAL_FLOAT(10.0f, filter->filter(nullptr, 5.0f));
    TEST_ASSERT_EQUAL_FLOAT(11.0f, filter->filter(nullptr, 6.0f));
    TEST_ASSERT_EQUAL_FLOAT(12.0f, filter->filter(nullptr, 7.0f));
}

void test_subtract_filter(void)
{
    IFilter<float>* filter = new SubtractFilter<float>(5.0f);

    TEST_ASSERT_EQUAL_FLOAT(0.0f, filter->filter(nullptr, 5.0f));
    TEST_ASSERT_EQUAL_FLOAT(-1.0f, filter->filter(nullptr, 4.0f));
    TEST_ASSERT_EQUAL_FLOAT(-2.0f, filter->filter(nullptr, 3.0f));
}

void test_multiply_filter(void)
{
    IFilter<float>* filter = new MultiplyFilter<float>(5.0f);

    TEST_ASSERT_EQUAL_FLOAT(25.0f, filter->filter(nullptr, 5.0f));
    TEST_ASSERT_EQUAL_FLOAT(30.0f, filter->filter(nullptr, 6.0f));
    TEST_ASSERT_EQUAL_FLOAT(35.0f, filter->filter(nullptr, 7.0f));
}

void test_voltage_divider_filter(void)
{
    IFilter<float>* filter = new VoltageDividerFilter(27000.0F, 100000.0F);

    TEST_ASSERT_EQUAL_FLOAT(0.0F, filter->filter(nullptr, 0.0F));
    TEST_ASSERT_EQUAL_FLOAT(3.429F, filter->filter(nullptr, 2.7F));
    TEST_ASSERT_EQUAL_FLOAT(3.81F, filter->filter(nullptr, 3.0F));
    TEST_ASSERT_EQUAL_FLOAT(4.191F, filter->filter(nullptr, 3.3F));
}

void test_clamp_filter(void)
{
    IFilter<float>* filter = new ClampFilter<float>(0.0f, 1.0f);

    TEST_ASSERT_EQUAL_FLOAT(0.0f, filter->filter(nullptr, -1.0f));
    TEST_ASSERT_EQUAL_FLOAT(0.0f, filter->filter(nullptr, 0.0f));
    TEST_ASSERT_EQUAL_FLOAT(0.5f, filter->filter(nullptr, 0.5f));
    TEST_ASSERT_EQUAL_FLOAT(1.0f, filter->filter(nullptr, 1.0f));
    TEST_ASSERT_EQUAL_FLOAT(1.0f, filter->filter(nullptr, 2.0f));
}

void test_lambda_filter(void)
{
    IFilter<float>* filter = new LambdaFilter<float>([](float value) {
        return value * 42.F; // Cause after all, 42 is the answer to everything.
    });

    TEST_ASSERT_EQUAL_FLOAT(42.0f, filter->filter(nullptr, 1.0f));
    TEST_ASSERT_EQUAL_FLOAT(84.0f, filter->filter(nullptr, 2.0f));
    TEST_ASSERT_EQUAL_FLOAT(126.0f, filter->filter(nullptr, 3.0f));
}

void test_sliding_window_moving_average_filter(void)
{
    IFilter<float>* filter = new SlidingWindowMovingAverageFilter<float>(3);

    TEST_ASSERT_EQUAL_FLOAT(1.0f, filter->filter(nullptr, 1.0f));   // 1 / 1 = 1
    TEST_ASSERT_EQUAL_FLOAT(1.5f, filter->filter(nullptr, 2.0f));   // (1 + 2) / 2 = 1.5
    TEST_ASSERT_EQUAL_FLOAT(2.0f, filter->filter(nullptr, 3.0f));   // (1 + 2 + 3) / 3 = 2
    TEST_ASSERT_EQUAL_FLOAT(3.0f, filter->filter(nullptr, 4.0f));   // (2 + 3 + 4) / 3 = 3
    TEST_ASSERT_EQUAL_FLOAT(4.0f, filter->filter(nullptr, 5.0f));   // (3 + 4 + 5) / 3 = 4
    TEST_ASSERT_EQUAL_FLOAT(5.0f, filter->filter(nullptr, 6.0f));   // (4 + 5 + 6) / 3 = 5
    TEST_ASSERT_EQUAL_FLOAT(6.0f, filter->filter(nullptr, 7.0f));   // (5 + 6 + 7) / 3 = 6
    TEST_ASSERT_EQUAL_FLOAT(7.0f, filter->filter(nullptr, 8.0f));   // (6 + 7 + 8) / 3 = 7
    TEST_ASSERT_EQUAL_FLOAT(8.0f, filter->filter(nullptr, 9.0f));   // (7 + 8 + 9) / 3 = 8
    TEST_ASSERT_EQUAL_FLOAT(9.0f, filter->filter(nullptr, 10.0f));  // (8 + 9 + 10) / 3 = 9
    TEST_ASSERT_EQUAL_FLOAT(10.0f, filter->filter(nullptr, 11.0f)); // (9 + 10 + 11) / 3 = 10

    ASSERT_EQUAL_FLOAT_ROUNDED(10.67f, filter->filter(nullptr, 11.0f), 2); // (10 + 11 + 11) / 3 = 10.67
    TEST_ASSERT_EQUAL_FLOAT(11.0f, filter->filter(nullptr, 11.0f));        // (11 + 11 + 11) / 3 = 11
    ASSERT_EQUAL_FLOAT_ROUNDED(11.33f, filter->filter(nullptr, 12.0f), 2); // (11 + 11 + 12) / 3 = 11.33
}

void test_exponential_moving_average_filter(void)
{
    IFilter<float>* filter = new ExponentialMovingAverageFilter<float>(0.5f);
    ASSERT_EQUAL_FLOAT_ROUNDED(1.0f, filter->filter(nullptr, 1.0f), 2);    // 1.0
    ASSERT_EQUAL_FLOAT_ROUNDED(1.5f, filter->filter(nullptr, 2.0f), 2);    // (0.5 * 1.0) + (0.5 * 2.0) = 1.5
    ASSERT_EQUAL_FLOAT_ROUNDED(2.25f, filter->filter(nullptr, 3.0f), 2);   // (0.5 * 1.5) + (0.5 * 3.0) = 2.25
    ASSERT_EQUAL_FLOAT_ROUNDED(3.125f, filter->filter(nullptr, 4.0f), 2);  // (0.5 * 2.25) + (0.5 * 4.0) = 3.125
    ASSERT_EQUAL_FLOAT_ROUNDED(4.0625f, filter->filter(nullptr, 5.0f), 2); // (0.5 * 3.125) + (0.5 * 5.0) = 4.0625

    filter = new ExponentialMovingAverageFilter<float>(0.1f);
    ASSERT_EQUAL_FLOAT_ROUNDED(1.0f, filter->filter(nullptr, 1.0f), 2);    // 1.0
    ASSERT_EQUAL_FLOAT_ROUNDED(1.1f, filter->filter(nullptr, 2.0f), 2);    // (0.1 * 2.0) + (0.9 * 1.0) = 1.1
    ASSERT_EQUAL_FLOAT_ROUNDED(1.29f, filter->filter(nullptr, 3.0f), 2);   // (0.1 * 3.0) + (0.9 * 1.1) = 1.29
    ASSERT_EQUAL_FLOAT_ROUNDED(1.561f, filter->filter(nullptr, 4.0f), 2);  // (0.1 * 4.0) + (0.9 * 1.29) = 1.561
    ASSERT_EQUAL_FLOAT_ROUNDED(1.9049f, filter->filter(nullptr, 5.0f), 2); // (0.1 * 5.0) + (0.9 * 1.561) = 1.9049

    filter = new ExponentialMovingAverageFilter<float>(0.9f);
    ASSERT_EQUAL_FLOAT_ROUNDED(1.0f, filter->filter(nullptr, 1.0f), 2);     // 1.0
    ASSERT_EQUAL_FLOAT_ROUNDED(1.9f, filter->filter(nullptr, 2.0f), 2);     // (0.9 * 2.0) + (0.1 * 1.0) = 1.9
    ASSERT_EQUAL_FLOAT_ROUNDED(2.89f, filter->filter(nullptr, 3.0f), 2);    // (0.9 * 3.0) + (0.1 * 1.9) = 2.89
    ASSERT_EQUAL_FLOAT_ROUNDED(3.889f, filter->filter(nullptr, 4.0f), 2);   // (0.9 * 4.0) + (0.1 * 2.89) = 3.889
    ASSERT_EQUAL_FLOAT_ROUNDED(4.8889f, filter->filter(nullptr, 5.0f), 2);  // (0.9 * 5.0) + (0.1 * 3.889) = 4.8889
    ASSERT_EQUAL_FLOAT_ROUNDED(4.98889, filter->filter(nullptr, 5.0f), 2);  // (0.9 * 5.0) + (0.1 * 4.8889) = 4.98889
    ASSERT_EQUAL_FLOAT_ROUNDED(4.99889, filter->filter(nullptr, 5.0f), 2);  // (0.9 * 5.0) + (0.1 * 4.98889) = 4.99889
    ASSERT_EQUAL_FLOAT_ROUNDED(4.999889, filter->filter(nullptr, 5.0f), 2); // (0.9 * 5.0) + (0.1 * 4.99889) = 4.999889
}

void test_center_deadzone_filter(void)
{
    IFilter<float>* filter = new CenterDeadzoneFilter(0.1f);

    TEST_ASSERT_EQUAL_FLOAT(0.0f, filter->filter(nullptr, 0.0f));
    TEST_ASSERT_EQUAL_FLOAT(0.1f, filter->filter(nullptr, 0.1f));

    // Inside the deadzone
    TEST_ASSERT_EQUAL_FLOAT(0.5f, filter->filter(nullptr, 0.43f));
    TEST_ASSERT_EQUAL_FLOAT(0.5f, filter->filter(nullptr, 0.5f));
    TEST_ASSERT_EQUAL_FLOAT(0.5f, filter->filter(nullptr, 0.57f));

    TEST_ASSERT_EQUAL_FLOAT(0.9f, filter->filter(nullptr, 0.9f));
    TEST_ASSERT_EQUAL_FLOAT(1.0f, filter->filter(nullptr, 1.0f));
}

void test_lookup_table_interpolate_linear_filter(void)
{
    const std::map<float, float> lookup_table = {
        { 0.0f, 0.0f }, { 1.0f, 3.5f }, { 2.0f, 7.0f }, { 3.0f, 10.5f }, { 4.0f, 14.0f }, { 5.0f, 17.5f },
    };
    IFilter<float>* filter = new LookupTableInterpolationFilter(lookup_table);

    // test existing values
    TEST_ASSERT_EQUAL_FLOAT(0.0f, filter->filter(nullptr, 0.0f));
    TEST_ASSERT_EQUAL_FLOAT(3.5f, filter->filter(nullptr, 1.0f));
    TEST_ASSERT_EQUAL_FLOAT(7.0f, filter->filter(nullptr, 2.0f));
    TEST_ASSERT_EQUAL_FLOAT(10.5f, filter->filter(nullptr, 3.0f));
    TEST_ASSERT_EQUAL_FLOAT(14.0f, filter->filter(nullptr, 4.0f));
    TEST_ASSERT_EQUAL_FLOAT(17.5f, filter->filter(nullptr, 5.0f));

    // test values in between
    TEST_ASSERT_EQUAL_FLOAT(1.75f, filter->filter(nullptr, 0.5f));
    TEST_ASSERT_EQUAL_FLOAT(5.25f, filter->filter(nullptr, 1.5f));
    TEST_ASSERT_EQUAL_FLOAT(8.75f, filter->filter(nullptr, 2.5f));
    TEST_ASSERT_EQUAL_FLOAT(12.25f, filter->filter(nullptr, 3.5f));
    TEST_ASSERT_EQUAL_FLOAT(15.75f, filter->filter(nullptr, 4.5f));

    // test out of range
    TEST_ASSERT_EQUAL_FLOAT(0.0f, filter->filter(nullptr, -1.0f));
    TEST_ASSERT_EQUAL_FLOAT(17.5f, filter->filter(nullptr, 6.0f));
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_add_filter);
    RUN_TEST(test_subtract_filter);
    RUN_TEST(test_multiply_filter);
    RUN_TEST(test_voltage_divider_filter);
    RUN_TEST(test_clamp_filter);
    RUN_TEST(test_lambda_filter);
    RUN_TEST(test_sliding_window_moving_average_filter);
    RUN_TEST(test_exponential_moving_average_filter);
    RUN_TEST(test_center_deadzone_filter);
    RUN_TEST(test_lookup_table_interpolate_linear_filter);

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
