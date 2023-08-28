#include <sensor/og_sensor.hpp>
#include <unity.h>

using namespace OpenGloves;

class TestAnalogSensor : public SenseShift::Input::ISimpleSensor<uint16_t> {
  private:
    uint16_t count = 0;

  public:
    int setupCounter = 0;

    void init() override { this->setupCounter++; };

    uint16_t getValue() override { return ++this->count; };
};

class TestBinarySensor : public SenseShift::Input::ISimpleSensor<bool> {
  public:
    bool value = false;
    int setupCounter = 0;

    void init() override { this->setupCounter++; };

    bool getValue() override { return this->value; };
};

class TestFingerSensor : public SenseShift::Input::ISimpleSensor<FingerValue> {
  public:
    FingerValue value;
    int setupCounter = 0;

    void init() override { this->setupCounter++; };

    FingerValue getValue() override { return this->value; };
};

void test_string_encoded_sensor_uint16(void)
{
    auto inner = new TestAnalogSensor();
    auto sensor = new StringEncodedMemoizedSensor<uint16_t>(inner, IEncodedInput::Type::INDEX);

    TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
    sensor->init();
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

void test_string_encoded_sensor_bool(void)
{
    auto inner = new TestBinarySensor();
    auto sensor = new StringEncodedMemoizedSensor<bool>(inner, IEncodedInput::Type::A_BTN);

    TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
    sensor->init();
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

void test_string_encoded_sensor_fingervalue(void)
{
    auto inner = new TestFingerSensor();
    auto sensor = new StringEncodedMemoizedSensor<FingerValue>(inner, IEncodedInput::Type::THUMB);

    TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
    sensor->init();
    TEST_ASSERT_EQUAL_INT(1, inner->setupCounter);

    // curl-only
    char buffer[sensor->getEncodedLength()];
    sensor->encodeString(buffer);
    TEST_ASSERT_EQUAL_STRING("A0", buffer);

    inner->value.curl = { 256 };
    sensor->updateValue();
    sensor->encodeString(buffer);
    TEST_ASSERT_EQUAL_STRING("A256", buffer);

    // curl + splay
    inner->value.splay = 420;
    sensor->updateValue();
    sensor->encodeString(buffer);
    TEST_ASSERT_EQUAL_STRING("A256(AB)420", buffer);

    // multi-curl
    inner->value.curl = { 128, 256, 512 };
    inner->value.splay = std::nullopt;
    sensor->updateValue();
    sensor->encodeString(buffer);
    TEST_ASSERT_EQUAL_STRING("A298(AAA)128(AAB)256(AAC)512", buffer); // 298 = (128 + 256 + 512) / 3

    // multi-curl + splay
    inner->value.splay = 69;
    sensor->updateValue();
    sensor->encodeString(buffer);
    TEST_ASSERT_EQUAL_STRING("A298(AAA)128(AAB)256(AAC)512(AB)69", buffer); // 298 = (128 + 256 + 512) / 3
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_string_encoded_sensor_uint16);
    RUN_TEST(test_string_encoded_sensor_bool);
    RUN_TEST(test_string_encoded_sensor_fingervalue);

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
