#include <unity.h>
#include <haptic_plane.hpp>

using namespace OH;

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

void test_it_sets_up_actuators(void) {
  oh_output_writers_map_t outputs = {
      {{0, 0}, new TestActuator()},
      {{0, 1}, new TestActuator()},
      {{1, 0}, new TestActuator()},
      {{1, 1}, new TestActuator()},
  };

  auto plane = new HapticPlane(outputs);
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

  auto plane = new HapticPlane(outputs);
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

void test_closest_it_writes_to_correct_if_exact(void) {
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

  auto plane = new HapticPlane_Closest(outputs);
  plane->setup();

  plane->writeOutput({{0, 0}, 1});
  plane->writeOutput({{0, 1}, 2});
  plane->writeOutput({{1, 0}, 3});
  plane->writeOutput({{1, 1}, 4});

  TEST_ASSERT_EQUAL(1, actuator->intensity);
  TEST_ASSERT_EQUAL(2, actuator2->intensity);
  TEST_ASSERT_EQUAL(3, actuator3->intensity);
  TEST_ASSERT_EQUAL(4, actuator4->intensity);
}

void test_closest_it_correctly_finds_closest(void) {
  auto actuator = new TestActuator(),
        actuator2 = new TestActuator(),
        actuator3 = new TestActuator(),
        actuator4 = new TestActuator();

  oh_output_writers_map_t outputs = {
      {{0, 0}, actuator},
      {{0, 64}, actuator2},
      {{64, 0}, actuator3},
      {{64, 64}, actuator4},
  };

  auto plane = new HapticPlane_Closest(outputs);
  plane->setup();

  plane->writeOutput({{16, 16}, 16});
  plane->writeOutput({{65, 65}, 65});

  TEST_ASSERT_EQUAL(16, actuator->intensity);
  TEST_ASSERT_EQUAL(0, actuator2->intensity);
  TEST_ASSERT_EQUAL(0, actuator3->intensity);
  TEST_ASSERT_EQUAL(65, actuator4->intensity);
}

void test_map_points(void) {
  auto point = mapPoint(0, 0, 0, 0);

  TEST_ASSERT_EQUAL(127, point->x);
  TEST_ASSERT_EQUAL(127, point->y);

  point = mapPoint(0, 0, 1, 1);

  TEST_ASSERT_EQUAL(85, point->x);
  TEST_ASSERT_EQUAL(85, point->y);

  point = mapPoint(1, 1, 1, 1);

  TEST_ASSERT_EQUAL(170, point->x);
  TEST_ASSERT_EQUAL(170, point->y);
}

int process(void) {
  UNITY_BEGIN();

  RUN_TEST(test_it_sets_up_actuators);
  RUN_TEST(test_it_writes_to_correct_output);
  RUN_TEST(test_closest_it_writes_to_correct_if_exact);
  RUN_TEST(test_closest_it_correctly_finds_closest);
  RUN_TEST(test_map_points);

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
