#include <calibration.hpp>
#include <unity.h>

using namespace OH;

void test_minmax_calibrator(void)
{
    auto calibrator = new MinMaxCalibrator<uint16_t, 0, 4096>();

    // test uncalibrated neutral value
    TEST_ASSERT_EQUAL_UINT16(2048, calibrator->calibrate(0));
    TEST_ASSERT_EQUAL_UINT16(2048, calibrator->calibrate(10));
    TEST_ASSERT_EQUAL_UINT16(2048, calibrator->calibrate(2048));
    TEST_ASSERT_EQUAL_UINT16(2048, calibrator->calibrate(4086));
    TEST_ASSERT_EQUAL_UINT16(2048, calibrator->calibrate(4096));

    calibrator->update(10);
    calibrator->update(4086);

    TEST_ASSERT_EQUAL_UINT16(0, calibrator->calibrate(0));
    TEST_ASSERT_EQUAL_UINT16(0, calibrator->calibrate(10));
    TEST_ASSERT_EQUAL_UINT16(118, calibrator->calibrate(128));
    TEST_ASSERT_EQUAL_UINT16(2048, calibrator->calibrate(2048));
    TEST_ASSERT_EQUAL_UINT16(3977, calibrator->calibrate(3968));
    TEST_ASSERT_EQUAL_UINT16(4096, calibrator->calibrate(4086));
    TEST_ASSERT_EQUAL_UINT16(4096, calibrator->calibrate(4096));

    calibrator->reset();

    // test uncalibrated neutral value (again)
    TEST_ASSERT_EQUAL_UINT16(2048, calibrator->calibrate(0));
    TEST_ASSERT_EQUAL_UINT16(2048, calibrator->calibrate(10));
    TEST_ASSERT_EQUAL_UINT16(2048, calibrator->calibrate(2048));
    TEST_ASSERT_EQUAL_UINT16(2048, calibrator->calibrate(4086));
    TEST_ASSERT_EQUAL_UINT16(2048, calibrator->calibrate(4096));
}

void test_fixed_center_point_deviation_calibrator(void)
{
    auto calibrator = new FixedCenterPointDeviationCalibrator<uint16_t, 512, 64, 0, 4096>();

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
    RUN_TEST(test_fixed_center_point_deviation_calibrator);

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
