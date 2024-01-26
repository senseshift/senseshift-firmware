#pragma once

#include <og_protocol.hpp>
#include <senseshift/input/sensor.hpp>

#include <optional>
#include <vector>

namespace OpenGloves {
    struct FingerValue {
        std::vector<uint16_t> curl = std::vector<uint16_t>({ 0 });
        std::optional<uint16_t> splay = std::nullopt;

        uint16_t getTotalCurl() const
        {
            if (this->curl.size() == 0) {
                return 0;
            }

            uint16_t total = 0;
            for (auto curl : this->curl) {
                total += curl;
            }
            return total / this->curl.size();
        }
    };

    template<typename _Tp>
    class StringEncodedMemoizedSensor :
      public IStringEncodedMemoizedSensor,
      public SenseShift::Input::MemoizedSensor<_Tp> {
      public:
        StringEncodedMemoizedSensor(SenseShift::Input::ISimpleSensor<_Tp>* sensor, IEncodedInput::Type type) :
          IStringEncodedMemoizedSensor(type), SenseShift::Input::MemoizedSensor<_Tp>(sensor){};

        void init() override { this->getSensor()->init(); }

        void updateValue() override { this->value = this->sensor_->getValue(); }

        [[nodiscard]] size_t getEncodedLength() const override;

        size_t encodeString(char* buffer) override;
    };

    template<>
    inline size_t StringEncodedMemoizedSensor<uint16_t>::getEncodedLength() const
    {
        return 6;
    }

    /// Format as "Axxxx", where A is the type and xxxxx is the value without leading zeros.
    template<>
    inline size_t StringEncodedMemoizedSensor<uint16_t>::encodeString(char* buffer)
    {
        return snprintf(buffer, this->getEncodedLength(), "%c%d", this->getType(), this->getValue());
    }

    template<>
    size_t StringEncodedMemoizedSensor<bool>::getEncodedLength() const
    {
        return 1;
    }

    template<>
    size_t StringEncodedMemoizedSensor<bool>::encodeString(char* buffer)
    {
        const auto value = this->getValue();

        if (value) {
            buffer[0] = this->getType();
        }

        return value ? this->getEncodedLength() : 0;
    }

    template<>
    size_t StringEncodedMemoizedSensor<FingerValue>::getEncodedLength() const
    {
        // curl + splay + (3 * knuckle)
        return 6 + 9 + (3 * 10);
    }

    template<>
    size_t StringEncodedMemoizedSensor<FingerValue>::encodeString(char* buffer)
    {
        size_t offset = 0;
        offset += snprintf(buffer + offset, 6, "%c%d", this->type, this->getValue().getTotalCurl());

        if (this->getValue().curl.size() > 1) {
            for (size_t i = 0; i < this->getValue().curl.size(); i++) {
                char knuckle = 'A' + i;
                offset += snprintf(buffer + offset, 10, "(%cA%c)%d", this->type, knuckle, this->getValue().curl[i]);
            }
        }

        if (this->getValue().splay.has_value()) {
            offset += snprintf(buffer + offset, 9, "(%cB)%d", this->type, this->getValue().splay.value());
        }

        return offset;
    }

} // namespace OpenGloves
