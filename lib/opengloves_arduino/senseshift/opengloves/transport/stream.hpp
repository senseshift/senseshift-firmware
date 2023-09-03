#pragma once

#include <BluetoothSerial.h>
#include <BLESerial.hpp>
#include <HardwareSerial.h>
#include <Print.h>
#include <WiFi.h>

#include <og_protocol.hpp>
#include <senseshift/opengloves/interface.hpp>

namespace SenseShift::OpenGloves {
    class IStreamTransport : public ITransport {
        using IStringEncodedMemoizedSensor = ::OpenGloves::IStringEncodedMemoizedSensor;

      protected:
        Stream* channel;
        char* buffer = new char[256];

      public:
        IStreamTransport(Stream* channel) : channel(channel){};

        size_t send(const char* buffer, size_t length) override
        {
            if (!this->isReady()) {
                return 0;
            }

            const auto written = this->channel->write(buffer, length);
            this->channel->flush();

            return written;
        }

        virtual bool isReady() = 0;

        virtual bool hasData() override
        {
            return this->isReady() && this->channel != nullptr && this->channel->available() > 0;
        }

        virtual size_t read(char* buffer, size_t length)
        {
            if (!this->hasData()) {
                return false;
            }

            size_t bytesRead = this->channel->readBytesUntil('\n', buffer, length);
            buffer[bytesRead] = '\0';

            return bytesRead;
        }
    };

    class StreamTransport : public IStreamTransport {
      public:
        StreamTransport(Stream& channel) : IStreamTransport(&channel){};
        StreamTransport(Stream* channel) : IStreamTransport(channel){};

        void setup() override { this->mReady = true; }

        bool isReady() override { return this->channel != nullptr && this->mReady; }

      private:
        bool mReady = false;
    };

    class BluetoothSerialTransport : public IStreamTransport {
      public:
        BluetoothSerialTransport(BluetoothSerial& channel) : IStreamTransport(&channel){};

        bool isReady() override
        {
            auto* serial = static_cast<BluetoothSerial*>(this->channel);
            return serial->isReady() && serial->hasClient();
        }

        virtual size_t send(const char* buffer, size_t length) override
        {
            auto written = this->channel->write(buffer, length);

            // TODO: This is a hack to ensure the data is sent
            delay(2);

            return written;
        }
    };

    class BLESerialTransport : public IStreamTransport {
      public:
        BLESerialTransport(BLESerial& channel) : IStreamTransport(&channel){};

        bool isReady() override
        {
            auto* serial = static_cast<BLESerial*>(this->channel);
            return serial->connected();
        }
    };

    class WiFiSerialTransport : public IStreamTransport {
      public:
        WiFiSerialTransport(WiFiServer& server) : IStreamTransport(nullptr), m_server(server){};

        void setup() override
        {
            auto* client = static_cast<WiFiClient*>(this->channel);
            if (client != nullptr) {
                if (client->connected()) {
                    return;
                }
            }

            this->m_server.begin();
            this->channel = new WiFiClient(this->m_server.available());
        }

        bool isReady() override
        {
            if (this->channel == nullptr) {
                return false;
            }

            auto* client = static_cast<WiFiClient*>(this->channel);
            return client->connected();
        }

      private:
        WiFiServer& m_server;
    };
} // namespace SenseShift::OpenGloves
