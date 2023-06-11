#pragma once

#include <sensor.hpp>
#include <calibration.hpp>
#include <og_protocol.hpp>

namespace OpenGloves
{
  template <typename _Tp>
  class StringEncodedMemoizedSensor : public IStringEncodedSensor, public OH::MemoizedSensor<_Tp> {
    public:
      StringEncodedMemoizedSensor(OH::ISensor<uint16_t>* sensor, IEncodedInput::Type type)
        : IStringEncodedSensor(type), OH::MemoizedSensor<uint16_t>(sensor) { };

    void setup() override {
      this->sensor->setup();
    }

    void updateValue() override {
      this->value = this->sensor->getValue();
    }

    size_t encodeString(char* buffer) override;
  };

  template <>
  size_t StringEncodedMemoizedSensor<uint16_t>::encodeString(char* buffer) {
    return snprintf(buffer, 6, "%c%d", this->getType(), this->getValue());
  }

  template <>
  size_t StringEncodedMemoizedSensor<bool>::encodeString(char* buffer) {
    if (value) {
      buffer[0] = this->getType();
    }
    return value ? 1 : 0;
  }

} // namespace OpenGloves
