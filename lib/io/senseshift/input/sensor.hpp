#pragma once

#include <type_traits>

#include <senseshift/calibration.hpp>
#include <senseshift/core/component.hpp>

#if defined(__AVR__)
#define ANALOG_MAX 1023
#elif defined(ESP32)
#define ANALOG_MAX 4095
#elif !defined(ANALOG_MAX)
#warning "This board doesn't have an auto ANALOG_MAX assignment, please set it manually"
#define ANALOG_MAX static_assert(false, "ANALOG_MAX is not defined")
// Uncomment and set as needed (only touch if you know what you are doing)
// #define ANALOG_MAX 4095
#endif

namespace SenseShift::Input {
    /// Abstract hardware sensor (e.g. potentiometer, flex sensor, etc.)
    /// \tparam Tp Type of the sensor value
    template<typename Tp>
    class ISimpleSensor : virtual public IInitializable {
      public:
        using ValueType = Tp;

        /// Get the current sensor value
        [[nodiscard]] virtual auto getValue() -> ValueType = 0;
    };

    using IBinarySensor = ISimpleSensor<bool>;
    using IFloatSensor = ISimpleSensor<float>;

    template<typename Tp>
    class ISensor : public virtual ISimpleSensor<Tp>, public ITickable {};

    /// Memoized sensor decorator. Stores the last read value and returns it on subsequent calls
    /// \tparam Tp Type of the sensor value
    template<typename Tp>
    class MemoizedSensor : public ISensor<Tp> {
      public:
        using ValueType = Tp;

        /// \param sensor Sensor to be decorated
        explicit MemoizedSensor(ISimpleSensor<ValueType>* sensor) : sensor_(sensor){}

        /**
         * Setup the sensor hardware
         */
        void init() override { this->sensor_->init(); }

        /**
         * Read actual value from the hardware and memoize it
         */
        void tick() override { this->value_ = this->sensor_->getValue(); }

        /**
         * Get the current memoized value
         */
        [[nodiscard]] auto getValue() -> ValueType override { return this->value_; }

    private:
        ISimpleSensor<ValueType>* sensor_;
        ValueType value_;
    };

    template<typename Tp>
    class ICalibratedSimpleSensor : public ISimpleSensor<Tp>, public Calibration::ICalibrated {};

    /// Calibrated sensor decorator
    /// \tparam Tp Type of the sensor value
    template<typename Tp>
    class CalibratedSimpleSensor : public ICalibratedSimpleSensor<Tp> {
      public:
        using ValueType = Tp;

        /// \param sensor Sensor to be decorated
        /// \param calibrator ICalibrator algorithm to be used
        CalibratedSimpleSensor(ISimpleSensor<ValueType>* sensor, Calibration::ICalibrator<ValueType>* calibrator) :
          sensor_(sensor), calibrator_(calibrator){};

        void init() override { this->sensor_->init(); };
        [[nodiscard]] auto getValue() -> ValueType override { return this->getCalibratedValue(); };

        void resetCalibration() override { this->calibrator_->reset(); };
        void enableCalibration() override { is_calibrating_ = true; }
        void disableCalibration() override { is_calibrating_ = false; }

      protected:
        [[nodiscard]] auto getCalibratedValue() -> ValueType
        {
            auto value = this->sensor_->getValue();

            if (this->is_calibrating_) {
                this->calibrator_->update(value);
            }

            return this->calibrator_->calibrate(value);
        }

    private:
        ISimpleSensor<ValueType>* sensor_;
        Calibration::ICalibrator<ValueType>* calibrator_;
        bool is_calibrating_ = false;
    };

    /// A sensor that returns the average value of N samples.
    /// \tparam Tp Type of the sensor value
    template<typename Tp>
    class AverageSensor : public ISimpleSensor<Tp> {
        static_assert(std::is_arithmetic_v<Tp>, "AverageSensor only supports arithmetic types");

      public:
        using ValueType = Tp;

        /// \param sensor Sensor to be decorated
        /// \param samples Number of samples to be used
        AverageSensor(ISimpleSensor<ValueType>* sensor, const size_t samples) : sensor_(sensor), samples_(samples) {}

        void init() override { this->sensor_->init(); };

        [[nodiscard]] auto getValue() const -> ValueType override
        {
            // TODO: another type for sum?
            double sum = 0;
            for (size_t i = 0; i < this->samples_; i++) {
                sum += this->sensor_->getValue();
            }

            return sum / this->samples_;
        }

      private:
        ISimpleSensor<ValueType>* sensor_;
        size_t samples_;
    };

    /// A sensor that returns the median value of N samples.
    /// \tparam Tp Type of the sensor value
    /// \tparam NumSamples Number of samples to be used
    template<typename Tp, size_t NumSamples>
    class StaticMedianSensor : public ISimpleSensor<Tp> {
        static_assert(std::is_arithmetic_v<Tp>, "StaticMedianSensor only supports arithmetic types");
        static_assert(NumSamples % 2 == 1, "StaticMedianSensor only supports odd sample sizes");

      public:
        using ValueType = Tp;

        explicit StaticMedianSensor(ISimpleSensor<ValueType>* sensor) : sensor_(sensor) {}

        void init() override { this->sensor_->init(); };

        [[nodiscard]] auto getValue() -> ValueType override
        {
            for (size_t i = 0; i < NumSamples; i++) {
                this->values_[i] = this->sensor_->getValue();
            }

            std::sort(this->values_.begin(), this->values_.end());

            return this->values_[NumSamples / 2];
        }

      private:
        /// Buffer to store the last N samples.
        /// We are using a static array to avoid dynamic allocation
        std::array<ValueType, NumSamples> values_;

        ISimpleSensor<Tp>* sensor_;
    };
} // namespace SenseShift::Input
