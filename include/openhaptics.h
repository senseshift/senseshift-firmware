#pragma once

#include "config/all.h"

#include <abstract_component.hpp>
#include <abstract_connection.hpp>
#include <output.hpp>
#include <events.hpp>

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include <abstract_battery.hpp>
#endif

#include <vector>

class OpenHaptics final : public OH::IComponentRegistry<OH::AbstractComponent>, public OH::IEventDispatcher
#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
, public OH::IBatteryConnected
#endif
{
 private:
  std::set<OH::AbstractComponent*> components{};
  OH::AbstractConnection* connection;
  OH::Output* output;

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  OH::AbstractBattery* battery;
#endif

 public:
  OpenHaptics();
  std::set<OH::AbstractComponent*> getComponents() {
    return this->components;
  };
  void registerComponent(OH::AbstractComponent*);

  OH::Output* getOutput() { return this->output; };
  void addOutputComponent(oh_output_path_t, OH::OutputComponent*);

  OH::AbstractConnection* getConnection() { return this->connection; };
  void setConnection(OH::AbstractConnection*);

  void postEvent(const OH::IEvent* event) override;

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  OH::AbstractBattery* getBattery() { return this->battery; };
  void setBattery(OH::AbstractBattery*);
#endif

  void setup(void);
  void loop(void);
};

extern OpenHaptics App;
