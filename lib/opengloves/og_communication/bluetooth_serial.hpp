#pragma once

#include "og_communication.hpp"
#include "og_communication/serial.hpp"

#include <BluetoothSerial.h>

namespace OG {
  class OG_BluetoothSerialCommunication : public OG_SerialCommunication<BluetoothSerial> {
    private:
      BluetoothSerial* serial;
      const char* deviceName;

    public:
      OG_BluetoothSerialCommunication(BluetoothSerial& serial, const char* deviceName)
        : OG_SerialCommunication<BluetoothSerial>(serial), serial(&serial), deviceName(deviceName) {};

      void begin() override {
        this->serial->begin(deviceName);
      };
  };
}
