#include "unity.h"

#ifdef UNIT_TEST

class TestWriter : public OutputWriter {
 public:
  void writeOutput(oh_output_intensity_t intensity) override{};
};

void test_coordinates(void) {
  oh_output_map_t testOutputs{
      {oh_output_point_t(10, 10), new TestWriter()},
      {oh_output_point_t(UINT16_MAX - 10, UINT16_MAX - 10), new TestWriter()},
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
