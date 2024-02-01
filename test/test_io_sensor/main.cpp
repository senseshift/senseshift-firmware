#include <senseshift/input/sensor.hpp>
#include <unity.h>

using namespace SenseShift::Input;

class TestAnalogCountingSensor : public ISimpleSensor<int> {
  public:
    int count = 0;
    int setupCounter = 0;

    void init() override { this->setupCounter++; };

    auto getValue() -> int override { return ++this->count; };
};

class TestAnalogSensor : public ISimpleSensor<int> {
public:
    int value = 0;
    int setupCounter = 0;

    void init() override { this->setupCounter++; };

    auto getValue() -> int override { return this->value; };
};

void test_memoized_sensor(void)
{
    auto inner = new TestAnalogCountingSensor();
    auto sensor = new SimpleSensorDecorator(inner);

    TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
    sensor->init();
    TEST_ASSERT_EQUAL_INT(1, inner->setupCounter);

    TEST_ASSERT_EQUAL_INT(0, sensor->getValue());
    TEST_ASSERT_EQUAL_INT(0, sensor->getValue());

    sensor->tick();

    TEST_ASSERT_EQUAL_INT(1, sensor->getValue());
    TEST_ASSERT_EQUAL_INT(1, sensor->getValue());
}

class DummyCalibrator : public ::SenseShift::Input::Calibration::ICalibrator<int> {
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
    auto inner = new TestAnalogCountingSensor();
    auto calibrator = new DummyCalibrator();

    auto sensor = new SimpleSensorDecorator(inner);
    sensor->setCalibrator(calibrator);

    TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
    sensor->init();
    TEST_ASSERT_EQUAL_INT(1, inner->setupCounter);

    calibrator->update(-1);
    sensor->tick();
    TEST_ASSERT_EQUAL_INT(-1, sensor->getValue());

    sensor->startCalibration();
    sensor->tick();
    TEST_ASSERT_EQUAL_INT(2, sensor->getValue());

    sensor->stopCalibration();
    sensor->tick();
    TEST_ASSERT_EQUAL_INT(2, sensor->getValue());

    sensor->reselCalibration();
    sensor->tick();
    TEST_ASSERT_EQUAL_INT(0, sensor->getValue());
    TEST_ASSERT_EQUAL_INT(1, calibrator->resetCounter);
}

void test_sensor_filter_multiply(void)
{
    auto inner = new TestAnalogSensor();
    auto sensor = new SimpleSensorDecorator(inner);
    sensor->addFilters({
      new ::SenseShift::Input::Filter::MultiplyFilter(2)
    });

    TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
    sensor->init();
    TEST_ASSERT_EQUAL_INT(1, inner->setupCounter);

    inner->value = 1;
    sensor->tick();
    TEST_ASSERT_EQUAL_INT(2, sensor->getValue());

    inner->value = 16;
    sensor->tick();
    TEST_ASSERT_EQUAL_INT(32, sensor->getValue());
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_memoized_sensor);
    RUN_TEST(test_calibrated_sensor);
    RUN_TEST(test_sensor_filter_multiply);

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
