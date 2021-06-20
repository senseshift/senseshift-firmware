#ifndef CONNECTION_H
#define CONNECTION_H

#include "../language/Language.h"

class Connection
{
  public:
      virtual bool isOpen(void) = 0;
      virtual void start(void) = 0;
      virtual void loop(Language*);
};

#endif /* CONNECTION_H */