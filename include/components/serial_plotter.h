#pragma once

#include <HardwareSerial.h>

#include <abstract_component.hpp>

class SerialPlotter_OutputStates : public OH::AbstractComponent {
 protected:
  HardwareSerial* serial;

 public:
  SerialPlotter_OutputStates(HardwareSerial& serial) : serial(&serial){};
  void setup(void) override;
  void loop(void) override;
};
