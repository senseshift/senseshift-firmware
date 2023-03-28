#pragma once

#include "og_communication.hpp"

#include <HardwareSerial.h>

namespace OG {

  /**
   * @tparam _Tp the type of the serial port
   */
  template <class _Tp>
  class OG_SerialCommunication : public ICommunication {
    private:
      _Tp* serial;

    public:
      OG_SerialCommunication(_Tp& serial) : serial(&serial) {};

      void send(char* data) override {
        this->serial->write(data);
      };
  };
} // namespace OG
