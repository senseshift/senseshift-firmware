#include "senseshift/input/calibration.hpp"
#include <unity.h>

using namespace SenseShift::Input::Calibration;

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

void test_minmax_calibrator(void)
{
    const auto calibrator = new MinMaxCalibrator<float>();

    // test uncalibrated neutral value
    TEST_ASSERT_EQUAL_FLOAT(0.5F, calibrator->calibrate(0));
    TEST_ASSERT_EQUAL_FLOAT(0.5F, calibrator->calibrate(10));
    TEST_ASSERT_EQUAL_FLOAT(0.5F, calibrator->calibrate(2048));
    TEST_ASSERT_EQUAL_FLOAT(0.5F, calibrator->calibrate(4086));
    TEST_ASSERT_EQUAL_FLOAT(0.5F, calibrator->calibrate(4096));

    calibrator->update(0.1F);
    calibrator->update(0.9F);

    TEST_ASSERT_EQUAL_FLOAT(0.0F, calibrator->calibrate(0.0F));
    TEST_ASSERT_EQUAL_FLOAT(0.0F, calibrator->calibrate(0.1F));
    TEST_ASSERT_EQUAL_FLOAT(0.125F, calibrator->calibrate(0.2F));
    TEST_ASSERT_EQUAL_FLOAT(0.25F, calibrator->calibrate(0.3F));
    TEST_ASSERT_EQUAL_FLOAT(0.375F, calibrator->calibrate(0.4F));
    TEST_ASSERT_EQUAL_FLOAT(0.5F, calibrator->calibrate(0.5F));
    TEST_ASSERT_EQUAL_FLOAT(0.625F, calibrator->calibrate(0.6F));
    TEST_ASSERT_EQUAL_FLOAT(0.75F, calibrator->calibrate(0.7F));
    TEST_ASSERT_EQUAL_FLOAT(0.875F, calibrator->calibrate(0.8F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, calibrator->calibrate(0.9F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, calibrator->calibrate(1.0F));

    calibrator->reset();

    // test uncalibrated neutral value (again)
    TEST_ASSERT_EQUAL_FLOAT(0.5F, calibrator->calibrate(0));
    TEST_ASSERT_EQUAL_FLOAT(0.5F, calibrator->calibrate(10));
    TEST_ASSERT_EQUAL_FLOAT(0.5F, calibrator->calibrate(2048));
    TEST_ASSERT_EQUAL_FLOAT(0.5F, calibrator->calibrate(4086));
    TEST_ASSERT_EQUAL_FLOAT(0.5F, calibrator->calibrate(4096));
}

void test_center_point_deviation_calibrator(void)
{
    auto calibrator = new CenterPointDeviationCalibrator<int>(100, 10, 0, 255);

    // Test reset function
    calibrator->reset();

    // Test update function
    calibrator->update(50);
    calibrator->update(75);
    calibrator->update(25);

    // Test calibrate function
    TEST_ASSERT_EQUAL_INT(255, calibrator->calibrate(100));
    TEST_ASSERT_EQUAL_INT(191, calibrator->calibrate(75));
    TEST_ASSERT_EQUAL_INT(63, calibrator->calibrate(50));
    TEST_ASSERT_EQUAL_INT(0, calibrator->calibrate(25));
    TEST_ASSERT_EQUAL_INT(0, calibrator->calibrate(0));
}

void test_fixed_center_point_deviation_calibrator(void)
{
    auto calibrator = new FixedCenterPointDeviationCalibrator<uint16_t>(512, 64, 0, 4096);

    // below deviation
    TEST_ASSERT_EQUAL_UINT16(0, calibrator->calibrate(0));
    TEST_ASSERT_EQUAL_UINT16(0, calibrator->calibrate(10));
    TEST_ASSERT_EQUAL_UINT16(0, calibrator->calibrate(1234));
    TEST_ASSERT_EQUAL_UINT16(0, calibrator->calibrate(1536));
    // center point
    TEST_ASSERT_EQUAL_UINT16(32, calibrator->calibrate(1544));
    TEST_ASSERT_EQUAL_UINT16(32, calibrator->calibrate(1550));
    TEST_ASSERT_EQUAL_UINT16(64, calibrator->calibrate(1555));
    TEST_ASSERT_EQUAL_UINT16(96, calibrator->calibrate(1560));
    TEST_ASSERT_EQUAL_UINT16(256, calibrator->calibrate(1600));
    TEST_ASSERT_EQUAL_UINT16(1056, calibrator->calibrate(1800));
    TEST_ASSERT_EQUAL_UINT16(1440, calibrator->calibrate(1900));
    TEST_ASSERT_EQUAL_UINT16(2048, calibrator->calibrate(2048));
    TEST_ASSERT_EQUAL_UINT16(2656, calibrator->calibrate(2200));
    TEST_ASSERT_EQUAL_UINT16(3232, calibrator->calibrate(2345));
    // above deviation
    TEST_ASSERT_EQUAL_UINT16(4096, calibrator->calibrate(4086));
    TEST_ASSERT_EQUAL_UINT16(4096, calibrator->calibrate(4096));

    calibrator->reset();
    calibrator->update(1234);
    calibrator->update(2345);

    // update does not change the calibration as the deviation is fixed

    // below deviation
    TEST_ASSERT_EQUAL_UINT16(0, calibrator->calibrate(0));
    TEST_ASSERT_EQUAL_UINT16(0, calibrator->calibrate(10));
    TEST_ASSERT_EQUAL_UINT16(0, calibrator->calibrate(1234));
    TEST_ASSERT_EQUAL_UINT16(0, calibrator->calibrate(1536));
    // center point
    TEST_ASSERT_EQUAL_UINT16(32, calibrator->calibrate(1544));
    TEST_ASSERT_EQUAL_UINT16(32, calibrator->calibrate(1550));
    TEST_ASSERT_EQUAL_UINT16(64, calibrator->calibrate(1555));
    TEST_ASSERT_EQUAL_UINT16(96, calibrator->calibrate(1560));
    TEST_ASSERT_EQUAL_UINT16(256, calibrator->calibrate(1600));
    TEST_ASSERT_EQUAL_UINT16(1056, calibrator->calibrate(1800));
    TEST_ASSERT_EQUAL_UINT16(1440, calibrator->calibrate(1900));
    TEST_ASSERT_EQUAL_UINT16(2048, calibrator->calibrate(2048));
    TEST_ASSERT_EQUAL_UINT16(2656, calibrator->calibrate(2200));
    TEST_ASSERT_EQUAL_UINT16(3232, calibrator->calibrate(2345));
    // above deviation
    TEST_ASSERT_EQUAL_UINT16(4096, calibrator->calibrate(4086));
    TEST_ASSERT_EQUAL_UINT16(4096, calibrator->calibrate(4096));
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_minmax_calibrator);
    RUN_TEST(test_center_point_deviation_calibrator);
    RUN_TEST(test_fixed_center_point_deviation_calibrator);

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
