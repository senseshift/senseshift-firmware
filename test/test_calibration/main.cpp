#include <unity.h>
#include <calibration.hpp>

using namespace OH;

void test_minmax_calibrator(void) {
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

int process(void) {
  UNITY_BEGIN();

  RUN_TEST(test_minmax_calibrator);

  return UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>

void setup(void) {
  process();
}

void loop(void) {}

#else

int main(int argc, char** argv) {
  return process();
}

#endif
