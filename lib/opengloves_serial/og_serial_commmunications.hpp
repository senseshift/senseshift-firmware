#pragma once

#include <og_protocol.hpp>
#include <HardwareSerial.h>

namespace OpenGloves
{
  class SerialCommunication : public ICommunication {
    private:
      HardwareSerial* serial;
      char* buffer = new char[256];

    public:
      SerialCommunication(HardwareSerial* serial) : serial(serial) { };

      void setup() override {
        this->serial->begin(115200);
      }

      void send(std::vector<IStringEncodedSensor*> sensors) override {
        // Encode all of the sensors into a single string.
        size_t length = encodeAll(buffer, sensors);
        // Send the encoded string over serial.
        this->serial->write(buffer, length);
      }

      static size_t encodeAll(char* buffer, std::vector<IStringEncodedSensor*> sensors) {
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
} // namespace OpenGloves
