
#include "language/LanguageBHapticsPlayer.cpp"

#if CONNECTION == CONNECTION_SERIAL
  #include "connection/ConnectionSerial.cpp"
  Connection* conn = new ConnectionSerial(SERIAL_USE);
#elif CONNECTION == CONNECTION_BLE_SERIAL
  #include "connection/ConnectionSerialBLE.cpp"
  Connection* conn = new ConnectionSerialBLE(BLE_SERIAL_USE);
#endif

Language* lang = new LanguageBHapticsPlayer();

void setup()
{
  conn->start();
}

void loop()
{
  if (conn->isOpen()) {
    conn->loop(lang);
  }
}
