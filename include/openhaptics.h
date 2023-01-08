#pragma once

#include <vector>

#include "component.h"
#include "config/all.h"
#include "connection.h"
#include "output.h"

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include "battery/abstract_battery.h"
#endif

class OpenHaptics final {
 private:
  std::vector<Component*> components{};
  Connection* connection;
  Output* output;

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  AbstractBattery* battery;
#endif

 public:
  OpenHaptics();
  std::vector<Component*> getComponents() { return this->components; };
  void registerComponent(Component*);

  Output* getOutput() { return this->output; };
  void addOutputComponent(outputPath_t, OutputComponent*);

  void setConnection(Connection*);
  Connection* getConnection() { return this->connection; };

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  void setBattery(AbstractBattery*);
  AbstractBattery* getBattery() { return this->battery; };
#endif

  void setup(void);
  void loop(void);
};

extern OpenHaptics App;
