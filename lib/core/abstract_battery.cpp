#include "abstract_battery.hpp"

#ifdef ARDUINO
#include <Arduino.h>
#endif

void OH::AbstractBattery::loop() {
  auto now_ms = millis();

  if (this->last_battery_sample == 0 || now_ms - this->last_battery_sample >= this->config.sampleRate) {
    this->last_battery_sample = now_ms;
    this->level = this->updateLevel();

    this->eventDispatcher->postEvent(new BatteryLevelEvent(this->level));
  }
};
