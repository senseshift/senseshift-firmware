#include "abstract_battery.hpp"

void OH::AbstractBattery::run() {
  while (true) {
    this->value = this->updateValue();
    this->eventDispatcher->postEvent(new BatteryLevelEvent(this->value));

    delay(this->rate);
  }
};
