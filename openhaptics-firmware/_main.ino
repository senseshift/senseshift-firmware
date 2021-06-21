
#if OH_CONNECTION == CONNECTION_BLE
  #include "connection/ble_bhaptics/ConnectionBLE.cpp"
  Connection* conn = new ConnectionBLE(BLUETOOTH_NAME);
#endif

void setup()
{
  conn->start();
}

void loop()
{
  if (conn->isOpen()) {
    // conn->loop(lang);
  }
}
