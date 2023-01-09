#include "unity.h"

#include "output_components/closest.h"

#ifdef UNIT_TEST

class TestWriter : public OutputWriter {
 public:
  void writeOutput(outputIntensity_t intensity) override{};
};

void test_coordinates(void) {
  outputMap_t testOutputs{
      {outputPoint_t(10, 10), new TestWriter()},
      {outputPoint_t(UINT16_MAX - 10, UINT16_MAX - 10), new TestWriter()},
  };

  auto output = new ClosestOutputComponent(testOutputs);

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

int main(int argc, char** argv) {
  return process();
}

#endif

#endif
