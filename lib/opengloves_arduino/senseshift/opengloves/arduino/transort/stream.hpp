#pragma once

#include <senseshift/opengloves/interface.hpp>

#include <BluetoothSerial.h>
#include <HardwareSerial.h>
#include <Print.h>
#include <WiFi.h>

namespace SenseShift::OpenGloves::Arduino {
    class IStreamTransport : public ITransport {
      protected:
        Stream* channel;
        char* buffer = new char[256];

      public:
        IStreamTransport(Stream* channel) : channel(channel){};

        size_t send(char* buffer, size_t length) override
        {
            if (!this->isReady()) {
                return;
            }

            const auto written = this->channel->write(buffer);
            this->channel->flush();

            return written;
        }

        virtual bool isReady() = 0;

        virtual bool hasData()
        {
            return this->isReady() && this->channel != nullptr && this->channel->available() > 0;
        }

        virtual size_t readCommand(char* buffer, size_t length)
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

        bool isReady() override { return this->channel != nullptr; }
    };

    class SerialTransport : public IStreamTransport {
      private:
        unsigned long baud;
        bool ready = false;

      public:
        SerialTransport(HardwareSerial& channel, unsigned long baud) : baud(baud), IStreamTransport(&channel){};

        void setup() override
        {
            if (this->ready) {
                return;
            }

            static_cast<HardwareSerial*>(this->channel)->begin(this->baud);
            this->ready = true;
        }

        bool isReady() override { return this->ready; }
    };

    class BluetoothSerialTransport : public IStreamTransport {
      private:
        std::string name;

      public:
        BluetoothSerialTransport(BluetoothSerial& channel, std::string name) : IStreamTransport(&channel), name(name){};

        void setup() override
        {
            auto* serial = static_cast<BluetoothSerial*>(this->channel);
            if (serial->isReady()) {
                return;
            }

            serial->begin(name.c_str());
        }

        bool isReady() override
        {
            auto* serial = static_cast<BluetoothSerial*>(this->channel);
            return serial->isReady() && serial->hasClient();
        }
    };

    class WiFiSerialTransport : public IStreamTransport
    {
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
} // namespace OpenGloves
