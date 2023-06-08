#include <unity.h>
#include <sensor.hpp>

using namespace OH;

class TestSensor : public ISensor<int> {
 private:
  int count = 0;

 public:
  int setupCounter = 0;

  void setup() override {
    this->setupCounter++;
  };

  int getValue() override {
    return ++this->count;
  };
};

void test_memoized_sensor(void) {
  auto inner = new TestSensor();
  auto sensor = new MemoizedSensor<int>(inner);

  TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
  sensor->setup();
  TEST_ASSERT_EQUAL_INT(1, inner->setupCounter);

  TEST_ASSERT_EQUAL_INT(0, sensor->getValue());
  TEST_ASSERT_EQUAL_INT(0, sensor->getValue());

  sensor->updateValue();

  TEST_ASSERT_EQUAL_INT(1, sensor->getValue());
  TEST_ASSERT_EQUAL_INT(1, sensor->getValue());
}

int process(void) {
  UNITY_BEGIN();

  RUN_TEST(test_memoized_sensor);

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
