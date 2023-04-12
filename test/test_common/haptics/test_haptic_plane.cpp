#include <unity.h>
#include <haptic_plane.hpp>

using namespace OH;

static HapticPlane* plane = nullptr;

class TestActuator : public AbstractActuator {
 public:
  bool isSetup = false;
  oh_output_intensity_t intensity = 0;

  TestActuator() : AbstractActuator() {}
  void setup() override {
    this->isSetup = true;
  }
  void writeOutput(oh_output_intensity_t intensity) override {
    this->intensity = intensity;
  }
};

void setUp(void) {
  //
}

void tearDown(void) {
  plane = nullptr;
}

void test_it_sets_up_actuators(void) {
  oh_output_writers_map_t outputs = {
      {{0, 0}, new TestActuator()},
      {{0, 1}, new TestActuator()},
      {{1, 0}, new TestActuator()},
      {{1, 1}, new TestActuator()},
  };

  plane = new HapticPlane(outputs);

  plane->setup();

  for (auto& kv : outputs) {
    TEST_ASSERT_TRUE(static_cast<TestActuator*>(kv.second)->isSetup);
  }
}

void test_it_writes_to_correct_output(void) {
  auto actuator = new TestActuator(),
        actuator2 = new TestActuator(),
        actuator3 = new TestActuator(),
        actuator4 = new TestActuator();

  oh_output_writers_map_t outputs = {
      {{0, 0}, actuator},
      {{0, 1}, actuator2},
      {{1, 0}, actuator3},
      {{1, 1}, actuator4},
  };

  plane = new HapticPlane(outputs);
  plane->setup();

  plane->writeOutput({{0, 0}, 64});
  plane->writeOutput({{0, 1}, 128});
  plane->writeOutput({{1, 0}, 192});
  plane->writeOutput({{1, 1}, 255});

  TEST_ASSERT_EQUAL_UINT8(64, actuator->intensity);
  TEST_ASSERT_EQUAL_UINT8(128, actuator2->intensity);
  TEST_ASSERT_EQUAL_UINT8(192, actuator3->intensity);
  TEST_ASSERT_EQUAL_UINT8(255, actuator4->intensity);
}

int process(void) {
  UNITY_BEGIN();

  RUN_TEST(test_it_sets_up_actuators);
  RUN_TEST(test_it_writes_to_correct_output);

  return UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>

void setup(void) {
  process();
}

void loop(void) {}

#else

int main(int argc, char** argv) {
  return process();
}

#endif
