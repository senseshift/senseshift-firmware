#include "unity.h"

#include "outputs/auto.h"

#define UNIT_TEST

#ifdef UNIT_TEST

class TestWriter : public OutputWriter
{
    public:
        void writeOutput(outputIntensity_t intensity) override {};
};

void test_coordinates(void)
{
    autoOutputVector_t frontOutputs {
        { new TestWriter(), new TestWriter(), },
        { new TestWriter(), new TestWriter(), },
    };

    OutputAutoComponent* output = new OutputAutoComponent(frontOutputs);

    auto points = output->getOutputPoints();
}

int process(void) {
    UNITY_BEGIN();

    RUN_TEST(test_coordinates);

    return UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>
void setup() {
    process();
}

void loop() {}

#else

int main(int argc, char **argv) {
    return process();
}

#endif

#endif