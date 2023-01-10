#pragma once

namespace OH {
  class AbstractComponent {
   public:
    virtual void setup(void){};
    virtual void loop(void){};
  };
}  // namespace OH
