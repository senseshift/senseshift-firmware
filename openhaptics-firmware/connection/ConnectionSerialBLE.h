#ifndef CONNECTION_SERIAL_BLE_H
#define CONNECTION_SERIAL_BLE_H

#include "ConnectionSerial.h"

#include <SoftwareSerial.h>

class ConnectionSerialBLE : public ConnectionSerial
{
  private:
    String deviceName;
  public:
    ConnectionSerialBLE(HardwareSerial* _serial) : ConnectionSerialBLE(_serial, "Tactsuit DIY") {};
    ConnectionSerialBLE(HardwareSerial*, const String &);
    void start(void);
};

#endif /* CONNECTION_BLE_H */