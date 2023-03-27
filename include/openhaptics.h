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

class OpenHaptics final : public OH::IComponentRegistry<OH::IComponent>, public OH::IEventDispatcher
{
 private:
  std::set<OH::IComponent*> components{};
  std::vector<const OH::IEventListener*> eventListeners{};
  OH::Output* output;

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  OH::AbstractBattery* battery;
#endif

 public:
  OpenHaptics();
  std::set<OH::IComponent*> getComponents() {
    return this->components;
  };
  void registerComponent(OH::IComponent*);

  OH::Output* getOutput() { return this->output; };

  void postEvent(const OH::IEvent* event) override;
  void addEventListener(const OH::IEventListener* listener) override;
};
