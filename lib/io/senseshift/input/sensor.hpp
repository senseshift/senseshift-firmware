#pragma once

#include <type_traits>

#include <senseshift/calibration.hpp>
#include <senseshift/interface.hpp>
#include <senseshift/logging.hpp>

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
    /**
     * Abstract hardware sensor (e.g. potentiometer, flex sensor, etc.)
     * @tparam _Tp Type of the sensor value
     */
    template<typename _Tp>
    struct ISimpleSensor : public virtual IInitializable {
        using ValueType = _Tp;

        /**
         * Get the current sensor value
         */
        virtual ValueType getValue() = 0;
    };

    template<typename _Tp>
    struct ISensor : public virtual ISimpleSensor<_Tp>, ITickable {};

    /**
     * Memoized sensor decorator
     * @tparam _Tp Type of the sensor value
     */
    template<typename _Tp>
    class MemoizedSensor : public ISensor<_Tp> {
      protected:
        ISimpleSensor<_Tp>* sensor;
        _Tp value;

      public:
        /**
         * @param sensor Sensor to be decorated
         */
        MemoizedSensor(ISimpleSensor<_Tp>* sensor) : sensor(sensor){};

        /**
         * Setup the sensor hardware
         */
        void init() override { this->sensor->init(); };

        /**
         * Read actual value from the hardware and memoize it
         */
        void tick() override { this->value = this->sensor->getValue(); };

        /**
         * Get the current memoized value
         */
        _Tp getValue() override { return this->value; };
    };

    template<typename _Tp>
    class ICalibratedSimpleSensor : public ISimpleSensor<_Tp>, public Calibration::ICalibrated {};

    /**
     * Calibrated sensor decorator
     *
     * @tparam _Tp Type of the sensor value
     */
    template<typename _Tp>
    class CalibratedSimpleSensor : public ICalibratedSimpleSensor<_Tp> {
      public:
        /**
         * @param sensor Sensor to be decorated
         * @param calibrator ICalibrator algorithm to be used
         */
        CalibratedSimpleSensor(ISimpleSensor<_Tp>* sensor, Calibration::ICalibrator<_Tp>* calibrator) :
          sensor(sensor), calibrator(calibrator){};

        void init() override { this->sensor->init(); };
        _Tp getValue() override { return this->getCalibratedValue(); };

        void resetCalibration() override { this->calibrator->reset(); };
        void enableCalibration() override { calibrating = true; }
        void disableCalibration() override { calibrating = false; }

      protected:
        ISimpleSensor<_Tp>* sensor;
        Calibration::ICalibrator<_Tp>* calibrator;
        bool calibrating = false;

        _Tp getCalibratedValue()
        {
            auto value = this->sensor->getValue();

            if (this->calibrating) {
                this->calibrator->update(value);
            }

            return this->calibrator->calibrate(value);
        }
    };

    template<typename _Tp>
    class AverageSensor : public ISimpleSensor<_Tp> {
        static_assert(std::is_arithmetic<_Tp>::value, "AverageSensor only supports arithmetic types");

      public:
        AverageSensor(ISimpleSensor<_Tp>* sensor, size_t samples) : sensor(sensor), samples(samples) {}

        void init() override { this->sensor->init(); };

        _Tp getValue() override
        {
            // TODO: another type for sum?
            double sum = 0;
            for (size_t i = 0; i < this->samples; i++) {
                sum += this->sensor->getValue();
            }

            return sum / this->samples;
        }

      private:
        ISimpleSensor<_Tp>* sensor;
        size_t samples;
    };
} // namespace SenseShift::Input
