#pragma once

namespace OH {
  namespace Core {
    class AbstractComponent {
     public:
      virtual void setup(void);
      virtual void loop(void);
    };
  }
}
