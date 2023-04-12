#include <unity.h>

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

int process(void) {
  UNITY_BEGIN();

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
