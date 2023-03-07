#pragma once

namespace OH
{
  template <typename _Tp>
  class ISensor {
   public:
    virtual _Tp getValue() = 0;
  };
} // namespace OH
