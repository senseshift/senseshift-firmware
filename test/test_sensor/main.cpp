#include <unity.h>
#include <sensor.hpp>

using namespace OH;

class TestAnalogSensor : public ISensor<int> {
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
  auto inner = new TestAnalogSensor();
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

class DummyCalibrator : public ICalibrator<int> {
  public:
    uint8_t resetCounter = 0;
    int calibrated = 0;

    void reset() override {
      this->resetCounter++;
      this->calibrated = 0;
    };
    void update(int input) override {
      this->calibrated = input;
    };
    int calibrate(int input) const override {
      return calibrated;
    };
};

void test_calibrated_sensor(void) {
  auto inner = new TestAnalogSensor();
  auto calibrator = new DummyCalibrator();
  auto sensor = new CalibratedSensor<int>(inner, calibrator);

  TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
  sensor->setup();
  TEST_ASSERT_EQUAL_INT(1, inner->setupCounter);

  calibrator->update(-1);
  TEST_ASSERT_EQUAL_INT(-1, sensor->getValue());

  sensor->enableCalibration();
  TEST_ASSERT_EQUAL_INT(2, sensor->getValue());

  sensor->disableCalibration();
  TEST_ASSERT_EQUAL_INT(2, sensor->getValue());

  sensor->resetCalibration();
  TEST_ASSERT_EQUAL_INT(0, sensor->getValue());
  TEST_ASSERT_EQUAL_INT(1, calibrator->resetCounter);
}

int process(void) {
  UNITY_BEGIN();

  RUN_TEST(test_memoized_sensor);
  RUN_TEST(test_calibrated_sensor);

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
