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

        this->m_pTxCharacteristic->setValue(const_cast<uint8_t*>(buffer), bufferSize);
        // this->flush();

        return bufferSize;
    }

    virtual size_t write(uint8_t byte) override
    {
        if (this->m_pTxCharacteristic == nullptr || !this->connected()) {
            return 0;
        }

        this->m_pTxCharacteristic->setValue(&byte, 1);
        // this->flush();

        return 1;
    }

    virtual void flush(void) override { this->m_pTxCharacteristic->notify(true); }

    /**
     * Begin BLE serial. This will create and start BLE server, service and characteristics.
     *
     * @note This will manage the BLE server, service and characteristics. If you want to manage them yourself, use the
     * other begin().
     *
     * @param deviceName Name of the BLE device
     * @param serviceUuid UUID of the BLE service
     * @param rxUuid UUID of the BLE characteristic for receiving data
     * @param txUuid UUID of the BLE characteristic for transmitting data
     */
    void begin(
      const char* deviceName,
      const char* serviceUuid = SERVICE_UUID,
      const char* rxUuid = RX_UUID,
      const char* txUuid = TX_UUID
    );

    /**
     * Begin BLE serial. This will create and start BLE service and characteristics.
     *
     * @note This will manage the BLE service and characteristics. If you want to manage them yourself, use the other
     * begin().
     *
     * @param pServer BLE server instance
     * @param serviceUuid UUID of the BLE service
     * @param rxUuid UUID of the BLE characteristic for receiving data
     * @param txUuid UUID of the BLE characteristic for transmitting data
     */
    void begin(
      BLEServer* pServer,
      const char* serviceUuid = SERVICE_UUID,
      const char* rxUuid = RX_UUID,
      const char* txUuid = TX_UUID
    )
    {
        BLEService* pService = pServer->getServiceByUUID(serviceUuid);
        if (pService == nullptr) {
            log_d("Creating BLE service with UUID '%s'", serviceUuid);
            pService = pServer->createService(serviceUuid);
        } else {
            log_w("BLE service with UUID '%s' already exists", serviceUuid);
        }

        // Store the service so we know if we're managing it
        this->m_pService = pService;

        this->begin(pService, rxUuid, txUuid);

        pService->start();
        log_d("Started BLE service");
    }

    /**
     * Begin BLE serial. This will create and start BLE characteristics.
     *
     * @note If you want to create characteristics yourself, use the other begin().
     *
     * @param pService BLE service instance
     * @param rxUuid UUID of the BLE characteristic for receiving data
     * @param txUuid UUID of the BLE characteristic for transmitting data
     */
    void begin(BLEService* pService, const char* rxUuid = RX_UUID, const char* txUuid = TX_UUID)
    {
        auto pRxCharacteristic = pService->getCharacteristic(rxUuid);
        if (pRxCharacteristic == nullptr) {
            log_d("Creating BLE characteristic with UUIDs '%s' (RX)", rxUuid);
            pRxCharacteristic = pService->createCharacteristic(rxUuid, BLECharacteristic::PROPERTY_WRITE_NR);
        } else {
            log_w("BLE characteristic with UUID '%s' (RX) already exists", rxUuid);
        }

        auto pTxCharacteristic = pService->getCharacteristic(txUuid);
        if (pTxCharacteristic == nullptr) {
            log_d("Creating BLE characteristic with UUIDs '%s' (TX)", txUuid);
            pTxCharacteristic = pService->createCharacteristic(
              txUuid,
              BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
            );
        } else {
            log_w("BLE characteristic with UUID '%s' (TX) already exists", txUuid);
        }

        this->begin(pRxCharacteristic, pTxCharacteristic);
    }

    /**
     * Begin BLE serial. This will setup the BLE characteristics.
     *
     * @param pServer BLE server instance
     * @param pRxCharacteristic BLE characteristic instance for receiving data
     * @param pTxCharacteristic BLE characteristic instance for transmitting data
     */
    void begin(BLECharacteristic* pRxCharacteristic, BLECharacteristic* pTxCharacteristic);

    void end()
    {
        if (this->m_pService != nullptr) {
            this->m_pService->stop();
        }

        if (this->m_pServer != nullptr) {
            this->m_pServer->getAdvertising()->stop();
        }

        this->m_pServer = nullptr;
    }

    bool connected() { return m_pServer != nullptr && m_pServer->getConnectedCount() > 0; }

    BLECharacteristic* getRxCharacteristic() { return m_pRxCharacteristic; }

    BLECharacteristic* getTxCharacteristic() { return m_pTxCharacteristic; }

  private:
    BLESerial(BLESerial const& other) = delete;      // disable copy constructor
    void operator=(BLESerial const& other) = delete; // disable assign constructor

    SenseShift::RingBuffer<uint8_t, BLESERIAL_RECEIVE_BUFFER_SIZE> m_receiveBuffer;

    /**
     * BLE server instance
     * @note This is only used if the BLESerial instance is managing the BLE server
     */
    BLEServer* m_pServer = nullptr;

    /**
     * BLE service instance
     * @note This is only used if the BLESerial instance is managing the BLE service
     */
    BLEService* m_pService = nullptr;

    /**
     * BLE characteristic instance for receiving data
     */
    BLECharacteristic* m_pRxCharacteristic = nullptr;

    /**
     * BLE characteristic instance for transmitting data
     */
    BLECharacteristic* m_pTxCharacteristic = nullptr;
};

class BLESerialServerCallbacks : public BLEServerCallbacks {
  public:
    BLESerialServerCallbacks(BLESerial* bleSerial) : bleSerial(bleSerial) {}

    void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t* param) override
    {
        uint16_t conn_id = param->connect.conn_id;
        pServer->updatePeerMTU(conn_id, BLESERIAL_ATTRIBUTE_MAX_VALUE_LENGTH);
    }

    void onDisconnect(BLEServer* pServer, esp_ble_gatts_cb_param_t* param) override
    {
        auto* pAdvertising = pServer->getAdvertising();
        if (pAdvertising == nullptr) {
            return;
        }
        pAdvertising->start();
    }

  private:
    BLESerial* bleSerial;
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
            bleSerial->m_receiveBuffer.push(rxValue[i]);
        }
    }

  private:
    BLESerial* bleSerial;
};

void BLESerial::begin(const char* deviceName, const char* serviceUuid, const char* rxUuid, const char* txUuid)
{
    // Create the BLE Device
    log_d("Initializing BLE device with name '%s'", deviceName);
    BLEDevice::init(deviceName);

    log_d("Creating BLE server");
    BLEServer* pServer = BLEDevice::createServer();
    pServer->setCallbacks(new BLESerialServerCallbacks(this));

    // Store the server so we know if we're managing it
    this->m_pServer = pServer;

    this->begin(pServer, serviceUuid, rxUuid, txUuid);

    BLEAdvertising* pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
    log_d("Started BLE advertising");
}

void BLESerial::begin(BLECharacteristic* pRxCharacteristic, BLECharacteristic* pTxCharacteristic)
{
    // Store the characteristics so we know if we're managing them
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
