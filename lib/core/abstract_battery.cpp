#include "abstract_battery.hpp"

#ifdef ARDUINO
#include <Arduino.h>
#endif

const std::string OH::BatteryLevelEvent::event = "battery_level";

void OH::AbstractBattery::loop() {
  auto now_ms = millis();

  if (now_ms - this->last_battery_sample >= this->config.sampleRate) {
    this->last_battery_sample = now_ms;
    this->level = this->updateLevel();
  }
};
