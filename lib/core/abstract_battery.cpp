#include "abstract_battery.hpp"

#ifdef ARDUINO
#include <Arduino.h>
#endif

void OH::AbstractBattery::run() {
  while (true) {
    this->value = this->updateValue();
    this->eventDispatcher->postEvent(new BatteryLevelEvent(this->value));

    delay(this->rate);
  }
};
