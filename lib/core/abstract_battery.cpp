#include "abstract_battery.hpp"
#include "logging.hpp"

#ifdef ARDUINO
#include <Arduino.h>
#endif

void OH::AbstractBattery::loop() {
  auto now_ms = millis();

  if (now_ms - this->last_battery_sample >= this->config.sampleRate) {
    this->last_battery_sample = now_ms;
    this->level = this->updateLevel();
  }
};
