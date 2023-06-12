#pragma once

#include <sensor.hpp>
#include <calibration.hpp>
#include <og_protocol.hpp>

namespace OpenGloves
{
  template <typename _Tp>
  class StringEncodedMemoizedSensor : public IStringEncodedSensor, public OH::MemoizedSensor<_Tp> {
    public:
      StringEncodedMemoizedSensor(OH::ISensor<_Tp>* sensor, IEncodedInput::Type type)
        : IStringEncodedSensor(type), OH::MemoizedSensor<_Tp>(sensor) { };

    void setup() override {
      this->sensor->setup();
    }

    void updateValue() override {
      this->value = this->sensor->getValue();
    }

    size_t getEncodedLength() const override;

    size_t encodeString(char* buffer) const override;
  };

  template <>
  size_t StringEncodedMemoizedSensor<uint16_t>::getEncodedLength() const {
    return 6;
  }

  template <>
  size_t StringEncodedMemoizedSensor<uint16_t>::encodeString(char* buffer) const {
    // Format as "Axxxxx", where A is the type and xxxxx is the value without leading zeros.
    return snprintf(buffer, this->getEncodedLength(), "%c%d", this->getType(), this->value);
  }

  template <>
  size_t StringEncodedMemoizedSensor<bool>::getEncodedLength() const {
    return 1;
  }

  template <>
  size_t StringEncodedMemoizedSensor<bool>::encodeString(char* buffer) const {
    if (value) {
      buffer[0] = this->getType();
    }
    return value ? this->getEncodedLength() : 0;
  }

} // namespace OpenGloves
