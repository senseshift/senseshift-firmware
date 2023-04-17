#include <unity.h>
#include <utility.hpp>

using namespace OH;

void test_contains_container(void) {
  std::vector<int> v = {1, 2, 3, 4, 5};
  TEST_ASSERT_TRUE(contains(v, 1));
  TEST_ASSERT_TRUE(contains(v, 2));
  TEST_ASSERT_TRUE(contains(v, 3));
  TEST_ASSERT_TRUE(contains(v, 4));
  TEST_ASSERT_TRUE(contains(v, 5));
  TEST_ASSERT_FALSE(contains(v, 6));
}

void test_contains_iterator(void) {
  int arr[] = {1, 2, 3, 4, 5};
  TEST_ASSERT_TRUE(contains(arr, arr + 5, 1));
  TEST_ASSERT_TRUE(contains(arr, arr + 5, 2));
  TEST_ASSERT_TRUE(contains(arr, arr + 5, 3));
  TEST_ASSERT_TRUE(contains(arr, arr + 5, 4));
  TEST_ASSERT_TRUE(contains(arr, arr + 5, 5));
  TEST_ASSERT_FALSE(contains(arr, arr + 5, 6));
}

void test_contains_string(void) {
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

  RUN_TEST(test_contains_container);
  RUN_TEST(test_contains_iterator);
  RUN_TEST(test_contains_string);
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
