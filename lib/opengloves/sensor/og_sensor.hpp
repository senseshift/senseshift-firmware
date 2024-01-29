#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include <og_protocol.hpp>
#include <senseshift/input/sensor.hpp>

namespace OpenGloves {
    struct FingerValue {
        std::vector<std::uint16_t> curl = std::vector<std::uint16_t>({ 0 });
        std::optional<std::uint16_t> splay = std::nullopt;

        [[nodiscard]] auto getTotalCurl() const -> std::uint16_t
        {
            if (this->curl.empty()) {
                return 0;
            }

            std::uint16_t total = 0;
            for (auto c_curl : this->curl) {
                total += c_curl;
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

        void updateValue() override { this->value_ = this->sensor_->getValue(); }

        [[nodiscard]] size_t getEncodedLength() const override;

        size_t encodeString(char* buffer) override;
    };
} // namespace OpenGloves
