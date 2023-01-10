#pragma once

#include <vector>

#include "config/all.h"

#include <components_registry.hpp>
#include <abstract_component.hpp>
#include <output.hpp>
#include <abstract_connection.hpp>

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include <abstract_battery.hpp>
#endif

class OpenHaptics final : public OH::IComponentRegistry<OH::AbstractComponent> {
 private:
  std::vector<OH::AbstractComponent*> components{};
  OH::AbstractConnection* connection;
  OH::Output* output;

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  OH::AbstractBattery* battery;
#endif

 public:
  OpenHaptics();
  std::vector<OH::AbstractComponent*> getComponents() { return this->components; };
  void registerComponent(OH::AbstractComponent*);

  OH::Output* getOutput() { return this->output; };
  void addOutputComponent(oh_output_path_t, OH::OutputComponent*);

  OH::AbstractConnection* getConnection() { return this->connection; };
  void setConnection(OH::AbstractConnection*);

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  OH::AbstractBattery* getBattery() { return this->battery; };
  void setBattery(OH::AbstractBattery*);
#endif

  void setup(void);
  void loop(void);
};

extern OpenHaptics App;
