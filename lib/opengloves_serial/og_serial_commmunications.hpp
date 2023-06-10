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

      void send(std::vector<IEncodedSensor*> sensors) override {
        int offset = 0;
        // Loop over all of the encoders and encode them to the output string.
        for (int i = 0; i < sensors.size(); i++) {
          // The offset is the total charecters already added to the string.
          offset += sensors[i]->encode(this->buffer + offset);
        }

        // Add a new line to the end of the encoded string.
        this->buffer[offset++] = '\n';
        this->buffer[offset] = '\0';

        this->serial->write(this->buffer, offset);
        this->serial->flush();
      }
  };
} // namespace OpenGloves
