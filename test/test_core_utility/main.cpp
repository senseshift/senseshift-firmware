#include <unity.h>
#include <utility.hpp>

using namespace OH;

void test_map(void) {
  TEST_ASSERT_EQUAL_UINT16(0, map(0, 0, 4095, 0, 255));
  TEST_ASSERT_EQUAL_UINT16(127, map(2047, 0, 4095, 0, 255));
  TEST_ASSERT_EQUAL_UINT16(255, map(4095, 0, 4095, 0, 255));

  TEST_ASSERT_EQUAL_UINT16(127, map(0, 0, 4095, 127, 255));
  TEST_ASSERT_EQUAL_UINT16(190, map(2047, 0, 4095, 127, 255));
  TEST_ASSERT_EQUAL_UINT16(255, map(4095, 0, 4095, 127, 255));

  TEST_ASSERT_EQUAL_UINT16(0, map(2048, 2048, 4095, 0, 255));
  TEST_ASSERT_EQUAL_UINT16(127, map(3071, 2048, 4095, 0, 255));
  TEST_ASSERT_EQUAL_UINT16(255, map(4095, 2048, 4095, 0, 255));
}

int process(void) {
  UNITY_BEGIN();

  RUN_TEST(test_map);

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
