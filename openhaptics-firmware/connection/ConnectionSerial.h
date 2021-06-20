#ifndef CONNECTION_SERIAL_H
#define CONNECTION_SERIAL_H

#include "./Connection.h"

class ConnectionSerial : public Connection
{
  private:
    HardwareSerial* serial;
    bool started;
  protected:
    HardwareSerial* getSerial(void);
  public:
    ConnectionSerial(HardwareSerial*);
    ~ConnectionSerial(void);
    
    bool isOpen(void);
    void start(void);
    void loop(Language*);
};

#endif /* CONNECTION_SERIAL_H */