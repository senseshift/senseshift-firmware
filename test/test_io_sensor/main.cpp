#include <senseshift/input/sensor.hpp>
#include <unity.h>

using namespace SenseShift::Input;
using namespace SenseShift::Calibration;

class TestAnalogSensor : public ISimpleSensor<int> {
  public:
    int count = 0;
    int setupCounter = 0;

    void init() override { this->setupCounter++; };

    int getValue() override { return ++this->count; };
};

void test_memoized_sensor(void)
{
    auto inner = new TestAnalogSensor();
    auto sensor = new MemoizedSensor<int>(inner);

    TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
    sensor->init();
    TEST_ASSERT_EQUAL_INT(1, inner->setupCounter);

    TEST_ASSERT_EQUAL_INT(0, sensor->getValue());
    TEST_ASSERT_EQUAL_INT(0, sensor->getValue());

    sensor->tick();

    TEST_ASSERT_EQUAL_INT(1, sensor->getValue());
    TEST_ASSERT_EQUAL_INT(1, sensor->getValue());
}

class DummyCalibrator : public ICalibrator<int> {
  public:
    uint8_t resetCounter = 0;
    int calibrated = 0;

    void reset() override
    {
        this->resetCounter++;
        this->calibrated = 0;
    };
    void update(int input) override { this->calibrated = input; };
    int calibrate(int input) const override { return calibrated; };
};

void test_calibrated_sensor(void)
{
    auto inner = new TestAnalogSensor();
    auto calibrator = new DummyCalibrator();
    auto sensor = new CalibratedSimpleSensor<int>(inner, calibrator);

    TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
    sensor->init();
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

void test_average_sensor(void)
{
    auto inner = new TestAnalogSensor();
    auto sensor = new AverageSensor<int>(inner, 3);

    TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
    sensor->init();
    TEST_ASSERT_EQUAL_INT(1, inner->setupCounter);

    // TODO: mock inner sensor, to return more interesting values
    TEST_ASSERT_EQUAL_INT(2, sensor->getValue());  // (1 + 2 + 3) / 3 = 2
    TEST_ASSERT_EQUAL_INT(5, sensor->getValue());  // (4 + 5 + 6) / 3 = 5
    TEST_ASSERT_EQUAL_INT(8, sensor->getValue());  // (7 + 8 + 9) / 3 = 8
    TEST_ASSERT_EQUAL_INT(11, sensor->getValue()); // (10 + 11 + 12) / 3 = 11

    inner->count = 0;
    sensor = new AverageSensor<int>(inner, 10);

    TEST_ASSERT_EQUAL_INT(5, sensor->getValue()); // (1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10) / 10 = 5
}

void test_static_median_sensor(void)
{
    auto inner = new TestAnalogSensor();
    auto sensor = new StaticMedianSensor<int, 3>(inner);

    TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
    sensor->init();
    TEST_ASSERT_EQUAL_INT(1, inner->setupCounter);

    // lmao, literally the same as average sensor
    // TODO: mock inner sensor, to return more interesting values
    TEST_ASSERT_EQUAL_INT(2, sensor->getValue());  // (1, 2, 3) = 2
    TEST_ASSERT_EQUAL_INT(5, sensor->getValue());  // (4, 5, 6) = 5
    TEST_ASSERT_EQUAL_INT(8, sensor->getValue());  // (7, 8, 9) = 8
    TEST_ASSERT_EQUAL_INT(11, sensor->getValue()); // (10, 11, 12) = 11
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_memoized_sensor);
    RUN_TEST(test_calibrated_sensor);
    RUN_TEST(test_average_sensor);
    RUN_TEST(test_static_median_sensor);

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
