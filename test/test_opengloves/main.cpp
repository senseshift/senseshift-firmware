#include <unity.h>
#include <sensor/og_sensor.hpp>

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

class TestBinarySensor : public OH::ISensor<bool> {
 public:
  bool value = false;
  int setupCounter = 0;

  void setup() override {
    this->setupCounter++;
  };

  bool getValue() override {
    return this->value;
  };
};

void test_string_encoded_sensor_uint16(void) {
  auto inner = new TestAnalogSensor();
  auto sensor = new StringEncodedMemoizedSensor<uint16_t>(inner, IEncodedInput::Type::INDEX);

  TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
  sensor->setup();
  TEST_ASSERT_EQUAL_INT(1, inner->setupCounter);

  TEST_ASSERT_EQUAL_INT(0, sensor->getValue());
  TEST_ASSERT_EQUAL_INT(0, sensor->getValue());

  sensor->updateValue();

  TEST_ASSERT_EQUAL_INT(1, sensor->getValue());
  TEST_ASSERT_EQUAL_INT(1, sensor->getValue());

  char buffer[6];
  TEST_ASSERT_EQUAL_INT(2, sensor->encodeString(buffer));
  TEST_ASSERT_EQUAL_STRING("B1", buffer);
}

void test_string_encoded_sensor_bool(void) {
  auto inner = new TestBinarySensor();
  auto sensor = new StringEncodedMemoizedSensor<bool>(inner, IEncodedInput::Type::A_BTN);

  TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
  sensor->setup();
  TEST_ASSERT_EQUAL_INT(1, inner->setupCounter);

  TEST_ASSERT_FALSE(sensor->getValue());

  char buffer[1];
  TEST_ASSERT_EQUAL_INT(0, sensor->encodeString(buffer));
  TEST_ASSERT_EQUAL_STRING("", buffer);

  sensor->updateValue();

  TEST_ASSERT_FALSE(sensor->getValue());

  inner->value = true;

  TEST_ASSERT_FALSE(sensor->getValue());

  sensor->updateValue();

  TEST_ASSERT_TRUE(sensor->getValue());

  TEST_ASSERT_EQUAL_INT(1, sensor->encodeString(buffer));
  TEST_ASSERT_EQUAL_STRING("J", buffer);
}

int process(void) {
  UNITY_BEGIN();

  RUN_TEST(test_string_encoded_sensor_uint16);
  RUN_TEST(test_string_encoded_sensor_bool);

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
