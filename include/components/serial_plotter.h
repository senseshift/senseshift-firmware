#pragma once

#include <HardwareSerial.h>

#include <abstract_component.hpp>

/**
 * Component, that prints the current state of the output to the serial port in Arduino's Serial Plotter format
 *
 * @tparam _Tp the type of the serial port
 */
template <class _Tp>
class SerialPlotter_OutputStates : public OH::AbstractComponent {
 private:
  _Tp* serial;
  uint32_t sampleRate;

 public:
  SerialPlotter_OutputStates(_Tp& serial, uint32_t sampleRate = 100)
    : OH::AbstractComponent({ "Serial Plotter", 1024, 255, tskNO_AFFINITY }),
      serial(&serial),
      sampleRate(sampleRate) {};

  void setup(void) override {};
  void loop(void) override;
};

/**
 * Specialized setup for HardwareSerial
 */
template<>
inline void SerialPlotter_OutputStates<HardwareSerial>::setup() {
  this->serial->begin(115200);
}

template class SerialPlotter_OutputStates<HardwareSerial>;
