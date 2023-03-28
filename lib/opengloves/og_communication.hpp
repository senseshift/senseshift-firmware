#pragma once

namespace OG {
  struct ICommunication {
    virtual void begin() = 0;

    virtual void send(char* data) = 0;
  };
}
