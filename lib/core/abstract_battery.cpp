#include "abstract_battery.hpp"

#ifdef ARDUINO
#include <Arduino.h>
#endif

void OH::AbstractBattery::loop() {
  this->level = this->updateLevel();
  this->eventDispatcher->postEvent(new BatteryLevelEvent(this->level));

  delay(this->config.sampleRate);
};
