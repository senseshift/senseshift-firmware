#include "ConnectionSerial.h"

#include <Arduino.h>

ConnectionSerial::ConnectionSerial(HardwareSerial* _serial)
{
  serial = _serial;
  started = false;
}

ConnectionSerial::~ConnectionSerial()
{
  serial->end();
}

HardwareSerial* ConnectionSerial::getSerial()
{
  return serial;
}

bool ConnectionSerial::isOpen()
{
  return started;
}

void ConnectionSerial::start()
{
  getSerial()->begin(SERIAL_BAUD_RATE);
  started = true;
}

void ConnectionSerial::loop(Language* lang)
{
  if (getSerial()->available()) {
    lang->decode(serial);
  }
}