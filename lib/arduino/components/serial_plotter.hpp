#pragma once

#include <abstract_component.hpp>
#include <output.hpp>

#include <HardwareSerial.h>

namespace OH {
  /**
   * Component, that prints the current state of the output to the serial port in Arduino's Serial Plotter format
   *
   * @tparam _Tp the type of the serial port
   */
  template <class _Tp>
  class SerialPlotter_OutputStates : public OH::TaskedComponent {
  private:
    _Tp* serial;
    Output* output;
    uint32_t sampleRate;

  public:
    SerialPlotter_OutputStates(_Tp& serial, Output* output, uint32_t sampleRate = 100, TaskConfig taskConfig = { "Serial Plotter", 1024, 1, tskNO_AFFINITY })
      : OH::TaskedComponent(taskConfig),
        serial(&serial),
        output(output),
        sampleRate(sampleRate) {};
    SerialPlotter_OutputStates(_Tp& serial, Output* output, uint32_t sampleRate = 100, const BaseType_t coreId = tskNO_AFFINITY)
      : OH::TaskedComponent({ "Serial Plotter", 1024, 1, coreId }),
        serial(&serial),
        output(output),
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
}
