#include "ConnectionSerial.cpp"
#include "ConnectionSerialBLE.h"

// List of AT commands: https://lesson.iarduino.ru/page/nastroyka-bluetooth-moduley-hc-06-hc-05-ble4-0/

ConnectionSerialBLE::ConnectionSerialBLE(HardwareSerial* _serial, const String &name) : ConnectionSerial(_serial) 
{
    deviceName = name;
}

void ConnectionSerialBLE::start()
{
    ConnectionSerial::start();
    getSerial()->println("AT+NAME" + deviceName);
}