#pragma once

#include <abstract_component.hpp>

class Connection : public OH::AbstractComponent {
 public:
  virtual void setup(void){};
  virtual void loop(void){};
};
