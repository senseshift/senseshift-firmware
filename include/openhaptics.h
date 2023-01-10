#pragma once

#include "config/all.h"

#include <abstract_component.hpp>
#include <abstract_connection.hpp>
#include <components_registry.hpp>
#include <output.hpp>

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include <abstract_battery.hpp>
#endif

#include <vector>

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
  std::vector<OH::AbstractComponent*> getComponents() {
    return this->components;
  };
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
