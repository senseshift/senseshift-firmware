#include "abstract_battery.hpp"

#ifdef ARDUINO
#include <Arduino.h>
#endif

void OH::AbstractBattery::run() {
  while (1) {
    this->level = this->updateLevel();
    this->eventDispatcher->postEvent(new BatteryLevelEvent(this->level));

    delay(this->config.sampleRate);
  }
};
