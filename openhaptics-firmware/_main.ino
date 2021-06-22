
#if OH_CONNECTION == CONNECTION_BLE
  #include "connection/ble_bhaptics/ConnectionBLE.cpp"
  Connection* conn = new ConnectionBLE(BLUETOOTH_NAME);
#endif

#if OH_OUTPUT == OUTPUT_VEST_X16
  #include "output/OutputBHX16.cpp"
  Output* output = new OutputBHX16();
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
