#pragma once

#include <BluetoothSerial.h>
#include <HardwareSerial.h>
#include <Print.h>
#include <WiFi.h>

#include <og_protocol.hpp>

namespace OpenGloves {
    class ISerialCommunication : public ICommunication {
      protected:
        Print& channel;
        char* buffer = new char[256];

      public:
        ISerialCommunication(Print& channel) : channel(channel){};

        void send(std::vector<IStringEncodedMemoizedSensor*>& sensors) override
        {
            if (!this->isReady()) {
                return;
            }

            // Encode all of the sensors into a single string.
            size_t length = encodeAll(buffer, sensors);
            // Send the encoded string over serial.
            this->channel.print(buffer);
            this->channel.flush();
        }

        virtual bool isReady() = 0;

        static size_t encodeAll(char* buffer, std::vector<IStringEncodedMemoizedSensor*>& sensors)
        {
            size_t offset = 0;
            // Loop over all of the encoders and encode them to the output string.
            for (size_t i = 0; i < sensors.size(); i++) {
                // The offset is the total charecters already added to the string.
                offset += sensors[i]->encodeString(buffer + offset);
            }

            // Add a new line to the end of the encoded string.
            buffer[offset++] = '\n';
            buffer[offset] = '\0';

            return offset;
        }

        virtual bool hasData() = 0;

        virtual size_t readCommand(char* buffer, size_t length) = 0;
    };

    class SerialCommunication : public ISerialCommunication {
      private:
        unsigned long baud;
        bool ready = false;

      public:
        SerialCommunication(HardwareSerial& channel, unsigned long baud) : baud(baud), ISerialCommunication(channel){};

        void setup() override
        {
            if (this->ready) {
                return;
            }

            static_cast<HardwareSerial&>(this->channel).begin(this->baud);
            this->ready = true;
        }

        bool isReady() override { return this->ready; }

        bool hasData() override
        {
            return this->isReady() && static_cast<HardwareSerial&>(this->channel).available() > 0;
        }

        size_t readCommand(char* buffer, size_t length) override
        {
            if (!this->hasData()) {
                return false;
            }

            size_t bytesRead = static_cast<HardwareSerial&>(this->channel).readBytesUntil('\n', buffer, length);
            buffer[bytesRead] = '\0';

            return bytesRead;
        }
    };

    class BTSerialCommunication : public ISerialCommunication {
      private:
        std::string name;

      public:
        BTSerialCommunication(BluetoothSerial& channel, std::string name) : ISerialCommunication(channel), name(name){};

        void setup() override
        {
            auto& serial = static_cast<BluetoothSerial&>(this->channel);
            if (serial.isReady()) {
                return;
            }

            // serial.register_callback([](esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
            //     log_i("Bluetooth event: %d", event);
            // });
            // serial.setTimeout(4);
            serial.begin(name.c_str());
        }

        bool isReady() override
        {
            auto& serial = static_cast<BluetoothSerial&>(this->channel);
            return serial.isReady() && serial.hasClient();
        }

        bool hasData() override
        {
            return this->isReady() && static_cast<BluetoothSerial&>(this->channel).available() > 0;
        }

        size_t readCommand(char* buffer, size_t length) override
        {
            if (!this->hasData()) {
                return false;
            }

            size_t bytesRead = static_cast<BluetoothSerial&>(this->channel).readBytesUntil('\n', buffer, length);
            buffer[bytesRead] = '\0';

            return bytesRead;
        }
    };
} // namespace OpenGloves
