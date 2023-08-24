#include <sensor/og_finger.hpp>
#include <unity.h>

using namespace OpenGloves;
using namespace SenseShift::Calibration;

class TestAnalogSensor : public SenseShift::Input::ISensor<uint16_t> {
  private:
    uint16_t count = 0;

  public:
    int setupCounter = 0;

    void setup() override { this->setupCounter++; };

    uint16_t getValue() override { return this->count++; };
};

class DummyCalibrator : public ICalibrator<uint16_t> {
  public:
    uint8_t resetCounter = 0;
    std::optional<uint16_t> calibrated = std::nullopt;

    void reset() override
    {
        this->resetCounter++;
        this->calibrated = std::nullopt;
    };
    void update(uint16_t input) override { this->calibrated = input; };
    uint16_t calibrate(uint16_t input) const override { return this->calibrated.value_or(input); };
};

void test_simple_finger_sensor_curl(void)
{
    auto* inner = new TestAnalogSensor();
    auto* calibrator = new DummyCalibrator();
    auto* calibrated = new SenseShift::Input::CalibratedSensor<uint16_t>(inner, calibrator);
    auto* sensor = new SimpleFingerSensor(calibrated);

    TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
    sensor->setup();
    TEST_ASSERT_EQUAL_INT(1, inner->setupCounter);

    // since the sensor is not memoized, the value is updated on every call
    TEST_ASSERT_EQUAL_INT(0, sensor->getValue().curl[0]);
    TEST_ASSERT_EQUAL_INT(1, sensor->getCurl());

    calibrator->calibrated = 100;

    TEST_ASSERT_EQUAL_INT(100, sensor->getValue().curl[0]); // 2 is inside
    TEST_ASSERT_EQUAL_INT(100, sensor->getCurl());          // 3 is inside

    calibrator->calibrated = std::nullopt;

    TEST_ASSERT_EQUAL_INT(4, sensor->getValue().curl[0]);
    TEST_ASSERT_EQUAL_INT(5, sensor->getCurl());
}

void test_simple_finger_sensor_curl_flex(void)
{
    auto* inner_curl = new TestAnalogSensor();
    auto* calibrator_curl = new DummyCalibrator();
    auto* calibrated_curl = new SenseShift::Input::CalibratedSensor<uint16_t>(inner_curl, calibrator_curl);

    auto* inner_flex = new TestAnalogSensor();
    auto* calibrator_flex = new DummyCalibrator();
    auto* calibrated_flex = new SenseShift::Input::CalibratedSensor<uint16_t>(inner_flex, calibrator_flex);

    auto* sensor = new SimpleFingerSensor(calibrated_curl, calibrated_flex);

    TEST_ASSERT_EQUAL_INT(0, inner_curl->setupCounter);
    TEST_ASSERT_EQUAL_INT(0, inner_flex->setupCounter);
    sensor->setup();
    TEST_ASSERT_EQUAL_INT(1, inner_curl->setupCounter);
    TEST_ASSERT_EQUAL_INT(1, inner_flex->setupCounter);

    // since the sensor is not memoized, the value is updated on every call
    TEST_ASSERT_EQUAL_INT(0, sensor->getValue().curl[0]);
    TEST_ASSERT_EQUAL_INT(1, sensor->getValue().splay.value());
    TEST_ASSERT_EQUAL_INT(2, sensor->getCurl());

    calibrator_curl->calibrated = 100;
    calibrator_flex->calibrated = 200;

    TEST_ASSERT_EQUAL_INT(100, sensor->getValue().curl[0]);       // 3 is inside
    TEST_ASSERT_EQUAL_INT(200, sensor->getValue().splay.value()); // 4 is inside
    TEST_ASSERT_EQUAL_INT(100, sensor->getCurl());                // 5 is inside

    calibrator_curl->calibrated = std::nullopt;
    calibrator_flex->calibrated = std::nullopt;

    TEST_ASSERT_EQUAL_INT(6, sensor->getValue().curl[0]);
    TEST_ASSERT_EQUAL_INT(7, sensor->getValue().splay.value());
    TEST_ASSERT_EQUAL_INT(8, sensor->getCurl());
}

// todo: tests for multi-curl

void test_finger_sensor_curl(void)
{
    auto* inner = new TestAnalogSensor();
    auto* calibrator = new DummyCalibrator();
    auto* calibrated = new SenseShift::Input::CalibratedSensor<uint16_t>(inner, calibrator);
    auto* sensor = new FingerSensor(calibrated, IEncodedInput::Type::INDEX);

    TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
    sensor->setup();
    TEST_ASSERT_EQUAL_INT(1, inner->setupCounter);

    TEST_ASSERT_EQUAL_INT(0, sensor->getValue().curl[0]);
    TEST_ASSERT_EQUAL_INT(0, sensor->getCurl());

    sensor->updateValue();

    TEST_ASSERT_EQUAL_INT(0, sensor->getValue().curl[0]);
    TEST_ASSERT_EQUAL_INT(0, sensor->getCurl());

    calibrator->calibrated = 100;

    TEST_ASSERT_EQUAL_INT(0, sensor->getValue().curl[0]);
    TEST_ASSERT_EQUAL_INT(0, sensor->getCurl());

    sensor->updateValue(); // +1 is inside

    TEST_ASSERT_EQUAL_INT(100, sensor->getValue().curl[0]);
    TEST_ASSERT_EQUAL_INT(100, sensor->getCurl());

    TEST_ASSERT_EQUAL_INT(0, calibrator->resetCounter);
    sensor->resetCalibration();
    TEST_ASSERT_EQUAL_INT(1, calibrator->resetCounter);

    sensor->enableCalibration();
    sensor->updateValue(); // +1 is inside
    TEST_ASSERT_EQUAL_INT(2, sensor->getValue().curl[0]);
    TEST_ASSERT_EQUAL_INT(2, sensor->getCurl());

    sensor->updateValue(); // +1 is inside
    TEST_ASSERT_EQUAL_INT(3, sensor->getValue().curl[0]);
    TEST_ASSERT_EQUAL_INT(3, sensor->getCurl());

    sensor->disableCalibration();
    sensor->updateValue(); // +1 is inside, but calibrated to 3
    TEST_ASSERT_EQUAL_INT(3, sensor->getValue().curl[0]);
    TEST_ASSERT_EQUAL_INT(3, sensor->getCurl());
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_simple_finger_sensor_curl);
    RUN_TEST(test_simple_finger_sensor_curl_flex);

    RUN_TEST(test_finger_sensor_curl);

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
