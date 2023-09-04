#pragma once
#ifndef BLESERIAL_H
#define BLESERIAL_H

#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include <algorithm>

#include <senseshift/buffer.hpp>

#define BLESERIAL_ATTRIBUTE_MAX_VALUE_LENGTH 20
#define BLESERIAL_RECEIVE_BUFFER_SIZE 256

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
        // uint16_t mtu = BLEDevice::getMTU();
        // uint16_t packetSize = mtu > 3 ? mtu - 3 : 20;

        // chunk the buffer into packets
        // for (size_t i = 0; i < bufferSize; i += packetSize) {
        //     auto chunkSize = static_cast<uint16_t>(std::min(static_cast<size_t>(packetSize), bufferSize - i));
        //     this->m_pTxCharacteristic->setValue(const_cast<uint8_t*>(buffer + i), chunkSize);
        //     this->flush();

        //     // delay if not last packet
        //     if (i + chunkSize < bufferSize) {
        //         delay(10);
        //     }
        // }

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
        auto pRxCharacteristic = pService->createCharacteristic(rxUuid, BLECharacteristic::PROPERTY_WRITE_NR);
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

    SenseShift::RingBuffer<uint8_t, BLESERIAL_RECEIVE_BUFFER_SIZE> m_receiveBuffer;

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
