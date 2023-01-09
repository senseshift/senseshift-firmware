#pragma once

#include <vector>

#include "config/all.h"

#include <abstract_component.hpp>
#include <output_component.hpp>
#include <abstract_connection.hpp>

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include <abstract_battery.hpp>
#endif

#include "output.h"

class OpenHaptics final {
 private:
  std::vector<OH::AbstractComponent*> components{};
  OH::AbstractConnection* connection;
  Output* output;

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  OH::AbstractBattery* battery;
#endif

 public:
  OpenHaptics();
  std::vector<OH::AbstractComponent*> getComponents() { return this->components; };
  void registerComponent(OH::AbstractComponent*);

  Output* getOutput() { return this->output; };
  void addOutputComponent(oh_output_path_t, OH::OutputComponent*);

  void setConnection(OH::AbstractConnection*);
  OH::AbstractConnection* getConnection() { return this->connection; };

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  void setBattery(OH::AbstractBattery*);
  OH::AbstractBattery* getBattery() { return this->battery; };
#endif

  void setup(void);
  void loop(void);
};

extern OpenHaptics App;
