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
    };

    class SerialCommunication : public ISerialCommunication {
      private:
        unsigned long baud;

      public:
        SerialCommunication(HardwareSerial& channel, unsigned long baud) : baud(baud), ISerialCommunication(channel){};

        bool isReady() override
        {
            return true;
        }

        void setup() override
        {
            static_cast<HardwareSerial&>(this->channel).begin(this->baud);
        }
    };

    class BTSerialCommunication : public ISerialCommunication {
      private:
        std::string name;

      public:
        BTSerialCommunication(BluetoothSerial& channel, std::string prefix) : ISerialCommunication(channel)
        {
            char suffix[4];
            sprintf(suffix, "%04X", (uint16_t) (ESP.getEfuseMac() >> 32));

            this->name = prefix + suffix;
        };

        bool isReady() override
        {
            return static_cast<BluetoothSerial&>(this->channel).hasClient();
        }

        void setup() override
        {
            auto& serial = static_cast<BluetoothSerial&>(this->channel);
            serial.begin(name.c_str());
        }
    };
} // namespace OpenGloves
