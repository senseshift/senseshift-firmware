#pragma once

#include <type_traits>

#include <calibration.hpp>
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
    class ISensor {
      public:
        /**
         * Setup the sensor hardware
         */
        virtual void setup() = 0;

        /**
         * Get the current sensor value
         */
        virtual _Tp getValue() = 0;
    };

    class IMemoizedSensor {
      public:
        /**
         * Update the memoized value
         */
        virtual void updateValue() = 0;
    };

    /**
     * Memoized sensor decorator
     * @tparam _Tp Type of the sensor value
     */
    template<typename _Tp>
    class MemoizedSensor : public ISensor<_Tp>, public virtual IMemoizedSensor {
      protected:
        ISensor<_Tp>* sensor;
        _Tp value;

      public:
        /**
         * @param sensor Sensor to be decorated
         */
        MemoizedSensor(ISensor<_Tp>* sensor) : sensor(sensor){};

        /**
         * Setup the sensor hardware
         */
        void setup() override
        {
            this->sensor->setup();
        };

        /**
         * Get the current memoized value
         */
        _Tp getValue() override
        {
            return this->value;
        };

        /**
         * Read actual value from the hardware and memoize it
         */
        void updateValue()
        {
            this->value = this->sensor->getValue();
        };
    };

    /**
     * Calibrated sensor decorator
     *
     * @tparam _Tp Type of the sensor value
     */
    template<typename _Tp>
    class CalibratedSensor : public ISensor<_Tp>, public OH::Calibrated {
      protected:
        ISensor<_Tp>* sensor;
        OH::ICalibrator<_Tp>* calibrator;

        _Tp getCalibratedValue()
        {
            auto value = this->sensor->getValue();

            if (this->calibrate) {
                this->calibrator->update(value);
            }

            return this->calibrator->calibrate(value);
        }

      public:
        /**
         * @param sensor Sensor to be decorated
         * @param calibrator ICalibrator algorithm to be used
         */
        CalibratedSensor(ISensor<_Tp>* sensor, OH::ICalibrator<_Tp>* calibrator) :
          sensor(sensor), calibrator(calibrator){};

        void setup() override
        {
            this->sensor->setup();
        };

        _Tp getValue() override
        {
            return this->getCalibratedValue();
        };

        void resetCalibration() override
        {
            this->calibrator->reset();
        };
    };
} // namespace SenseShift::Input
