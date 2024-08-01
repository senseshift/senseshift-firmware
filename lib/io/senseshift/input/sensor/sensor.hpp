#pragma once

#include <optional>
#include <type_traits>
#include <vector>

#include "senseshift/input/calibration.hpp"
#include "senseshift/input/filter.hpp"

#include <senseshift/core/component.hpp>
#include <senseshift/core/helpers.hpp>

#define SS_SUBSENSOR_INIT(SENSOR, ATTACH_CALLBACK, CALLBACK) \
    (SENSOR)->init();                                        \
    if (ATTACH_CALLBACK) {                                   \
        (SENSOR)->addValueCallback(CALLBACK);                \
    }

namespace SenseShift::Input {
/// Abstract hardware sensor (e.g. potentiometer, flex sensor, etc.)
/// \tparam Tp Type of the sensor value
template<typename Tp>
class ISimpleSensor : public virtual IInitializable {
  public:
    using ValueType = Tp;

    explicit ISimpleSensor() = default;

    /// Get the current sensor value.
    [[nodiscard]] virtual auto getValue() -> ValueType = 0;
};

using IBinarySimpleSensor = ISimpleSensor<bool>;
using IFloatSimpleSensor = ISimpleSensor<float>;

template<typename Tp>
class ISensor : public virtual ISimpleSensor<Tp>, public Calibration::Calibrated<Tp>, public Filter::Filtered<Tp> {};

template<typename Tp>
class Sensor : public ISensor<Tp>, public Component {
  public:
    using ValueType = Tp;
    using CallbackManagerType = CallbackManager<void(ValueType)>;
    using CallbackType = typename CallbackManagerType::CallbackType;

    explicit Sensor() = default;

    template<typename U = Tp, std::enable_if_t<std::is_same_v<U, float>, int> = 0>
    explicit Sensor(float value = 0.0f) : raw_value_(value)
    {
        this->value_ = this->applyFilters(value);
    }

    void addValueCallback(CallbackType&& callback)
    {
        this->callbacks_.add(std::move(callback));
    }

    void addRawValueCallback(CallbackType&& callback)
    {
        this->raw_callbacks_.add(std::move(callback));
    }

    void init() override
    {
    }

    /// Publish the given state to the sensor.
    ///
    /// Firstly, the given state will be assigned to the sensor's raw_value_.
    /// Then, the raw_value_ will be passed through the sensor's filter chain.
    /// Finally, the filtered value will be assigned to the sensor's .value_.
    ///
    /// \param rawValue The new .raw_value_.
    void publishState(ValueType rawValue)
    {
        this->raw_value_ = rawValue;
        this->raw_callbacks_.call(this->raw_value_);

        this->value_ = this->applyFilters(rawValue);
        this->callbacks_.call(this->value_);
    }

    /// Get the current sensor .value_.
    [[nodiscard]] auto getValue() -> ValueType override
    {
        return this->value_;
    }

    /// Get the current raw sensor .raw_value_.
    [[nodiscard]] auto getRawValue() -> ValueType
    {
        return this->raw_value_;
    }

  protected:
    /// Apply current filters to value.
    [[nodiscard]] auto applyFilters(ValueType value) -> ValueType
    {
        /// Apply calibration
        if (this->getCalibrator() != nullptr) {
            if (this->isCalibrating()) {
                this->getCalibrator()->update(value);
            }

            value = this->getCalibrator()->calibrate(value);
        }

        /// Apply filters
        for (auto filter : this->getFilters()) {
            value = filter->filter(nullptr, value);
        }

        return value;
    }

  private:
    friend class Filter::IFilter<ValueType>;
    friend class Calibration::ICalibrator<ValueType>;

    ValueType raw_value_;
    ValueType value_;

    /// Storage for raw state callbacks.
    CallbackManagerType raw_callbacks_;
    /// Storage for filtered state callbacks.
    CallbackManagerType callbacks_;
};

using FloatSensor = Sensor<float>;

// todo: support double/triple/N-times/long click and so on
using BinarySensor = Sensor<bool>;

template<typename Tp>
class SimpleSensorDecorator : public Sensor<Tp> {
  public:
    using ValueType = Tp;
    using SourceType = ISimpleSensor<ValueType>;

    explicit SimpleSensorDecorator(SourceType* source) : source_(source), Sensor<Tp>()
    {
    }

    void init() override
    {
        this->source_->init();
    }

    void tick() override
    {
        this->updateValue();
    }

    auto updateValue() -> ValueType
    {
        auto const raw_value = this->readRawValue();
        this->publishState(raw_value);

        LOG_D("decorator.simple", " raw_value=%f, value=%f", raw_value, this->getValue());

        return this->getValue();
    }

    [[nodiscard]] auto readRawValue() -> ValueType
    {
        return this->source_->getValue();
    }

  protected:
  private:
    SourceType* source_;
};

namespace _private {
class TheFloatSensor : public Sensor<float> {};
} // namespace _private
} // namespace SenseShift::Input
