#pragma once

#include <array>

#include <BLESerial.h>
#include <BluetoothSerial.h>
#include <HardwareSerial.h>
#include <Print.h>
#include <WiFi.h>

#include <senseshift/opengloves/opengloves.hpp>

namespace SenseShift::OpenGloves {
class IStreamTransport : public ITransport {
  protected:
    Stream* channel;
    std::array<char, 256> buffer_{};

  public:
    IStreamTransport(Stream* channel) : channel(channel)
    {
    }

    auto send(const char* buffer, size_t length) -> size_t override
    {
        if (!this->isReady()) {
            return 0;
        }

        const auto written = this->channel->write(buffer, length);
        this->channel->flush();

        return written;
    }

    virtual auto isReady() -> bool = 0;

    auto hasData() -> bool override
    {
        return this->isReady() && this->channel != nullptr && this->channel->available() > 0;
    }

    auto read(char* buffer, size_t length) -> size_t override
    {
        if (!this->hasData()) {
            return 0U;
        }

        size_t bytesRead = this->channel->readBytesUntil('\n', buffer, length);
        buffer[bytesRead] = '\0';

        return bytesRead;
    }
};

class StreamTransport : public IStreamTransport {
  public:
    explicit StreamTransport(Stream& channel) : IStreamTransport(&channel)
    {
    }
    explicit StreamTransport(Stream* channel) : IStreamTransport(channel)
    {
    }

    void init() override
    {
        this->mReady = true;
    }

    auto isReady() -> bool override
    {
        return this->channel != nullptr && this->mReady;
    }

  private:
    bool mReady = false;
};

class BluetoothSerialTransport : public IStreamTransport {
  public:
    explicit BluetoothSerialTransport(BluetoothSerial& channel) : IStreamTransport(&channel){};

    auto isReady() -> bool override
    {
        auto* serial = static_cast<BluetoothSerial*>(this->channel);
        return serial->isReady() && serial->hasClient();
    }

    void init() override
    {
    }

    auto send(const char* buffer, size_t length) -> size_t override
    {
        auto written = this->channel->write(buffer, length);

        // TODO: This is a hack to ensure the data is sent
        delay(2);

        return written;
    }
};

template<typename T = std::queue<uint8_t>>
class BLESerialTransport : public IStreamTransport {
  public:
    explicit BLESerialTransport(BLESerial<T>& channel) : IStreamTransport(&channel){};

    void init() override
    {
    }

    auto isReady() -> bool override
    {
        auto* serial = static_cast<BLESerial<T>*>(this->channel);
        return serial->connected();
    }
};

class WiFiSerialTransport : public IStreamTransport {
  public:
    WiFiSerialTransport(WiFiServer& server) : IStreamTransport(nullptr), m_server(server){};

    void init() override
    {
        auto* client = static_cast<WiFiClient*>(this->channel);
        if (client != nullptr) {
            if (client->connected() != 0U) {
                return;
            }
        }

        this->m_server.begin();
        this->channel = new WiFiClient(this->m_server.accept());
    }

    auto isReady() -> bool override
    {
        if (this->channel == nullptr) {
            return false;
        }

        auto* client = static_cast<WiFiClient*>(this->channel);
        return client->connected() != 0U;
    }

  private:
    WiFiServer& m_server;
};
} // namespace SenseShift::OpenGloves
