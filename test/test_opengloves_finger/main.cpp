#include <unity.h>
#include <sensor/og_finger.hpp>

using namespace OpenGloves;

class TestAnalogSensor : public OH::ISensor<uint16_t> {
 private:
  uint16_t count = 0;

 public:
  int setupCounter = 0;

  void setup() override {
    this->setupCounter++;
  };

  uint16_t getValue() override {
    return ++this->count;
  };
};

class DummyCalibrator : public OH::ICalibrator<uint16_t> {
  public:
    uint8_t resetCounter = 0;
    uint16_t calibrated = 0;

    void reset() override {
      this->resetCounter++;
      this->calibrated = 0;
    };
    void update(uint16_t input) override {
      this->calibrated = input;
    };
    uint16_t calibrate(uint16_t input) const override {
      return calibrated;
    };
};

void test_finger_sensor_curl(void) {
  auto* inner = new TestAnalogSensor();
  auto* calibrator = new DummyCalibrator();
  auto* calibrated = new OH::CalibratedSensor<uint16_t>(inner, calibrator);
  auto* sensor = new FingerSensor(calibrated, IEncodedInput::Type::INDEX);

  TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
  sensor->setup();
  TEST_ASSERT_EQUAL_INT(1, inner->setupCounter);

  TEST_ASSERT_EQUAL_INT(0, sensor->getValue());
  TEST_ASSERT_EQUAL_INT(0, sensor->getCurl());

  sensor->updateValue();

  TEST_ASSERT_EQUAL_INT(0, sensor->getValue());
  TEST_ASSERT_EQUAL_INT(0, sensor->getCurl());

  calibrator->calibrated = 100;

  TEST_ASSERT_EQUAL_INT(0, sensor->getValue());
  TEST_ASSERT_EQUAL_INT(0, sensor->getCurl());

  sensor->updateValue();

  TEST_ASSERT_EQUAL_INT(100, sensor->getValue());
  TEST_ASSERT_EQUAL_INT(100, sensor->getCurl());

  TEST_ASSERT_EQUAL_INT(0, calibrator->resetCounter);
  sensor->resetCalibration();
  TEST_ASSERT_EQUAL_INT(1, calibrator->resetCounter);

  sensor->enableCalibration();
  sensor->updateValue();
  TEST_ASSERT_EQUAL_INT(3, sensor->getValue());
  TEST_ASSERT_EQUAL_INT(3, sensor->getCurl());

  sensor->updateValue();
  TEST_ASSERT_EQUAL_INT(4, sensor->getValue());
  TEST_ASSERT_EQUAL_INT(4, sensor->getCurl());

  sensor->disableCalibration();
  sensor->updateValue();
  TEST_ASSERT_EQUAL_INT(4, sensor->getValue());
  TEST_ASSERT_EQUAL_INT(4, sensor->getCurl());
}

int process(void) {
  UNITY_BEGIN();

  RUN_TEST(test_finger_sensor_curl);

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
