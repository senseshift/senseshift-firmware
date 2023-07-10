#include <haptic_body.hpp>
#include <unity.h>

using namespace OH;

class TestActuator : public AbstractActuator {
  public:
    bool isSetup = false;
    oh_output_intensity_t intensity = 0;

    TestActuator() : AbstractActuator() {}
    void setup() override
    {
        this->isSetup = true;
    }
    void writeOutput(oh_output_intensity_t intensity) override
    {
        this->intensity = intensity;
    }
};

void test_it_sets_up_planes(void)
{
    auto body = new HapticBody();

    oh_output_writers_map_t outputs = {
        { { 0, 0 }, new TestActuator() },
        { { 0, 1 }, new TestActuator() },
        { { 1, 0 }, new TestActuator() },
        { { 1, 1 }, new TestActuator() },
    };
    auto plane = new HapticPlane(outputs);

    body->addComponent(0, plane);
    body->setup();

    for (auto& kv : outputs) {
        TEST_ASSERT_TRUE(static_cast<TestActuator*>(kv.second)->isSetup);
    }
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_it_sets_up_planes);

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
