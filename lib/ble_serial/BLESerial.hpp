#pragma once
#ifndef BLESERIAL_H
#define BLESERIAL_H

#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#define BLESERIAL_ATTRIBUTE_MAX_VALUE_LENGTH 20
#define BLESERIAL_RECEIVE_BUFFER_SIZE 256

template<typename _Tp, size_t N>
class ByteRingBuffer {
  public:
    using ValueType = _Tp;

    void add(ValueType value)
    {
        ringBuffer[newestIndex] = value;
        newestIndex = (newestIndex + 1) % N;
        length = min(length + 1, N);
    }

    int pop()
    { // pops the oldest value off the ring buffer
        if (length == 0) {
            return -1;
        }
        ValueType result = ringBuffer[(N + newestIndex - length) % N];
        length -= 1;
        return result;
    }

    void clear()
    {
        newestIndex = 0;
        length = 0;
    }

    int get(size_t index)
    { // this.get(0) is the oldest value, this.get(this.getLength() - 1) is the newest value
        if (index < 0 || index >= length) {
            return -1;
        }
        return ringBuffer[(N + newestIndex - length + index) % N];
    }

    size_t getLength() { return length; }

  private:
    ValueType ringBuffer[N];
    size_t newestIndex = 0;
    size_t length = 0;
};

class BLESerialCharacteristicCallbacks;
class BLESerialServerCallbacks;

class BLESerial : public Stream {
    friend class BLESerialCharacteristicCallbacks;
    friend class BLESerialServerCallbacks;

  public:
    inline static const char* SERVICE_UUID = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
    inline static const char* RX_UUID = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
    inline static const char* TX_UUID = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";

    BLESerial() : m_receiveBuffer() {}

    virtual int available() override { return m_receiveBuffer.getLength(); }

    virtual int peek() override { return m_receiveBuffer.get(0); }

    virtual int read() override { return m_receiveBuffer.pop(); }

    virtual size_t write(const uint8_t* buffer, size_t bufferSize) override
    {
        if (this->m_pTxCharacteristic == nullptr || !this->connected()) {
            return 0;
        }

        this->m_pTxCharacteristic->setValue(const_cast<uint8_t*>(buffer), bufferSize);
        this->flush();

        return bufferSize;
    }

    virtual size_t write(uint8_t byte) override
    {
        if (this->m_pTxCharacteristic == nullptr || !this->connected()) {
            return 0;
        }

        this->m_pTxCharacteristic->setValue(&byte, 1);
        this->flush();

        return 1;
    }

    virtual void flush(void) override { this->m_pTxCharacteristic->notify(true); }

    void begin(
      const char* deviceName,
      const char* serviceUuid = SERVICE_UUID,
      const char* rxUuid = RX_UUID,
      const char* txUuid = TX_UUID
    )
    {
        // Create the BLE Device
        log_d("Creating BLE device with name '%s'", deviceName);
        BLEDevice::init(deviceName);

        BLEServer* pServer = BLEDevice::createServer();

        this->begin(pServer, serviceUuid, rxUuid, txUuid);
    }

    void begin(
      BLEServer* pServer,
      const char* serviceUuid = SERVICE_UUID,
      const char* rxUuid = RX_UUID,
      const char* txUuid = TX_UUID
    )
    {
        log_d("Creating BLE service with UUID '%s'", serviceUuid);
        BLEService* pService = pServer->getServiceByUUID(serviceUuid);
        if (pService == nullptr) {
            pService = pServer->createService(serviceUuid);
        }

        log_d("Creating BLE characteristics with UUIDs '%s' (RX) and '%s' (TX)", rxUuid, txUuid);
        auto pRxCharacteristic = pService->createCharacteristic(rxUuid, BLECharacteristic::PROPERTY_WRITE);
        auto pTxCharacteristic =
          pService->createCharacteristic(txUuid, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

        this->begin(pServer, pRxCharacteristic, pTxCharacteristic);

        pService->start();
        log_d("Started BLE service");

        BLEAdvertising* pAdvertising = pServer->getAdvertising();
        pAdvertising->start();
        log_d("Started BLE advertising");
    }

    void begin(BLEServer* pServer, BLECharacteristic* pRxCharacteristic, BLECharacteristic* pTxCharacteristic);

    bool connected() { return m_pServer != nullptr && m_pServer->getConnectedCount() > 0; }

  private:
    BLESerial(BLESerial const& other) = delete;      // disable copy constructor
    void operator=(BLESerial const& other) = delete; // disable assign constructor

    ByteRingBuffer<uint8_t, BLESERIAL_RECEIVE_BUFFER_SIZE> m_receiveBuffer;

    BLEServer* m_pServer;
    BLECharacteristic* m_pRxCharacteristic;
    BLECharacteristic* m_pTxCharacteristic;
};

class BLESerialServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) override {}

    void onDisconnect(BLEServer* pServer) override
    {
        auto* pAdvertising = pServer->getAdvertising();
        if (pAdvertising == nullptr) {
            return;
        }
        pAdvertising->start();
    }
};

class BLESerialCharacteristicCallbacks : public BLECharacteristicCallbacks {
  public:
    BLESerialCharacteristicCallbacks(BLESerial* bleSerial) : bleSerial(bleSerial) {}

    void onWrite(BLECharacteristic* pCharacteristic)
    {
        if (pCharacteristic != bleSerial->m_pRxCharacteristic) {
            return;
        }

        std::string rxValue = pCharacteristic->getValue();
        for (int i = 0; i < rxValue.length(); i++) {
            bleSerial->m_receiveBuffer.add(rxValue[i]);
        }
    }

  private:
    BLESerial* bleSerial;
};

void BLESerial::begin(BLEServer* pServer, BLECharacteristic* pRxCharacteristic, BLECharacteristic* pTxCharacteristic)
{
    this->m_pServer = pServer;
    this->m_pRxCharacteristic = pRxCharacteristic;
    this->m_pTxCharacteristic = pTxCharacteristic;

    // this->m_pRxCharacteristic->setAccessPermissions(ESP_GATT_PERM_WRITE_ENCRYPTED);
    this->m_pRxCharacteristic->addDescriptor(new BLE2902());
    this->m_pRxCharacteristic->setCallbacks(new BLESerialCharacteristicCallbacks(this));
    this->m_pRxCharacteristic->setWriteProperty(true);

    // this->m_pTxCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED);
    this->m_pTxCharacteristic->addDescriptor(new BLE2902());
    this->m_pTxCharacteristic->setReadProperty(true);
    this->m_pTxCharacteristic->setNotifyProperty(true);
}

#endif // BLESERIAL_H
