#include <senseshift/battery/input/battery_sensor.hpp>
#include <senseshift/input/sensor.hpp>
#include <unity.h>

using namespace SenseShift::Input;
using namespace SenseShift::Battery;
using namespace SenseShift::Battery::Input;

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

void test_battery_sensor(void)
{
    auto* source = new FloatSensor();

    auto* battery = new LookupTableInterpolateBatterySensor(source, &VoltageMap::LiPO_1S_42);
    battery->init();

    source->publishState(0.f);
    TEST_ASSERT_EQUAL_INT(0, battery->getValue().level);

    source->publishState(4.2f);
    TEST_ASSERT_EQUAL_INT(255, battery->getValue().level);

    source->publishState(3.7f);
    TEST_ASSERT_EQUAL_INT(31, battery->getValue().level);
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_battery_sensor);

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

int main()
{
    return process();
}

#endif
