#include <senseshift/input/sensor/analog_threshold.hpp>
#include <senseshift/input/sensor/sensor.hpp>
#include <unity.h>

using namespace SenseShift::Input;

class TestAnalogCountingSensor : public ISimpleSensor<int> {
  public:
    int count = 0;
    int setupCounter = 0;

    void init() override
    {
        this->setupCounter++;
    }

    auto getValue() -> int override
    {
        return ++this->count;
    }
};

class TestAnalogSensor : public ISimpleSensor<int> {
  public:
    int value = 0;
    int setupCounter = 0;

    void init() override
    {
        this->setupCounter++;
    };

    auto getValue() -> int override
    {
        return this->value;
    };
};

class TestFloatSensor : public ISimpleSensor<float> {
  public:
    float value = 0.0f;
    int setupCounter = 0;

    void init() override
    {
        this->setupCounter++;
    };

    auto getValue() -> float override
    {
        return this->value;
    };
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

class DummyCalibrator : public ::SenseShift::Input::Calibration::ICalibrator<float> {
  public:
    uint8_t resetCounter = 0;
    float calibrated = 0.0f;

    void reset() override
    {
        this->resetCounter++;
        this->calibrated = 0.0f;
    }
    void update(float input) override
    {
        this->calibrated = input;
    }
    float calibrate(float input) const override
    {
        return calibrated;
    }
};

void test_calibrated_sensor(void)
{
    auto inner = new FloatSensor();
    auto calibrator = new DummyCalibrator();

    auto sensor = new SimpleSensorDecorator(inner);
    sensor->setCalibrator(calibrator);

    calibrator->update(-1.0f);
    sensor->publishState(0.0f);
    TEST_ASSERT_EQUAL_FLOAT(-1.0f, sensor->getValue());

    sensor->publishState(100.0f);
    TEST_ASSERT_EQUAL_FLOAT(-1.0f, sensor->getValue());

    calibrator->update(2.0f);
    sensor->publishState(102.0f);
    TEST_ASSERT_EQUAL_FLOAT(2.0f, sensor->getValue());

    sensor->startCalibration();
    sensor->publishState(200.0f);
    TEST_ASSERT_EQUAL_FLOAT(200.0f, sensor->getValue());

    sensor->publishState(202.0f);
    TEST_ASSERT_EQUAL_FLOAT(202.0f, sensor->getValue());

    sensor->stopCalibration();
    sensor->publishState(300.0f);
    TEST_ASSERT_EQUAL_FLOAT(202.0f, sensor->getValue());
}

void test_sensor_filter_multiply(void)
{
    auto inner = new TestAnalogSensor();
    auto sensor = new SimpleSensorDecorator(inner);
    sensor->addFilters({ new ::SenseShift::Input::Filter::MultiplyFilter(2) });

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

void test_sensor_filter_center_deadzone(void)
{
    auto inner = new TestFloatSensor();
    auto sensor = new SimpleSensorDecorator(inner);
    sensor->addFilters({ new ::SenseShift::Input::Filter::CenterDeadzoneFilter(0.1, 0.5) });

    TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
    sensor->init();
    TEST_ASSERT_EQUAL_INT(1, inner->setupCounter);

    inner->value = 0;
    sensor->tick();
    TEST_ASSERT_EQUAL_FLOAT(0.0f, sensor->getValue());

    inner->value = 0.2;
    sensor->tick();
    TEST_ASSERT_EQUAL_FLOAT(0.2f, sensor->getValue());

    inner->value = 0.45;
    sensor->tick();
    TEST_ASSERT_EQUAL_FLOAT(0.5f, sensor->getValue());

    inner->value = 0.55;
    sensor->tick();
    TEST_ASSERT_EQUAL_FLOAT(0.5f, sensor->getValue());

    inner->value = 0.8;
    sensor->tick();
    TEST_ASSERT_EQUAL_FLOAT(0.8f, sensor->getValue());
}

void test_sensor_multiple_filters(void)
{
    auto inner = new TestAnalogSensor();
    auto sensor = new SimpleSensorDecorator(inner);

    // Test MultiplyFilter separately
    sensor->addFilter(new ::SenseShift::Input::Filter::MultiplyFilter(2));
    // Test AddFilter separately
    sensor->addFilter(new ::SenseShift::Input::Filter::AddFilter(1));

    TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
    sensor->init();
    TEST_ASSERT_EQUAL_INT(1, inner->setupCounter);

    inner->value = 1;
    sensor->tick();
    TEST_ASSERT_EQUAL_INT(3, sensor->getValue());

    inner->value = 16;
    sensor->tick();
    TEST_ASSERT_EQUAL_INT(33, sensor->getValue());
}

void test_sensor_analog_threshold(void)
{
    auto inner = new TestAnalogSensor();
    auto source = new SimpleSensorDecorator(inner);
    auto sensor = new AnalogThresholdSensor(source, 120, 80, true);

    TEST_ASSERT_EQUAL_INT(0, inner->setupCounter);
    sensor->init();
    TEST_ASSERT_EQUAL_INT(1, inner->setupCounter);

    // 100 is below the threshold, so the sensor should be off
    inner->value = 100;
    source->tick();
    TEST_ASSERT_FALSE(sensor->getValue());

    // 130 is above the threshold, so the sensor should be on
    inner->value = 130;
    source->tick();
    TEST_ASSERT_TRUE(sensor->getValue());

    // 90 is below the upper threshold, but above the lower threshold, so the sensor should stay on due to hysteresis
    inner->value = 90;
    source->tick();
    TEST_ASSERT_TRUE(sensor->getValue());

    // 70 is below the lower threshold, so the sensor should be off
    inner->value = 70;
    source->tick();
    TEST_ASSERT_FALSE(sensor->getValue());
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_memoized_sensor);
    RUN_TEST(test_calibrated_sensor);
    RUN_TEST(test_sensor_filter_multiply);
    RUN_TEST(test_sensor_filter_center_deadzone);
    RUN_TEST(test_sensor_multiple_filters);
    RUN_TEST(test_sensor_analog_threshold);

    return UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>

void setup(void)
{
    process();
}

void loop(void)
{
}

#else

int main(int argc, char** argv)
{
    return process();
}

#endif
