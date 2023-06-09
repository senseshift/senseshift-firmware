#pragma once

#include <type_traits>

#include <calibration.hpp>

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

namespace OH
{
  /**
   * Abstract hardware sensor (e.g. potentiometer, flex sensor, etc.)
   * @tparam _Tp Type of the sensor value
   */
  template <typename _Tp>
  class ISensor {
   public:
   /**
    * Setup the sensor hardware
    */
    virtual void setup() {};

    /**
     * Get the current sensor value
     */
    virtual _Tp getValue() = 0;
  };

  /**
   * Memoized sensor decorator
   * @tparam _Tp Type of the sensor value
   */
  template <typename _Tp>
  class MemoizedSensor : public ISensor<_Tp> {
   protected:
    ISensor<_Tp>* sensor;
    _Tp value;

   public:
    /**
     * @param sensor Sensor to be decorated
     */
    MemoizedSensor(ISensor<_Tp>* sensor) : sensor(sensor) {};

    /**
     * Setup the sensor hardware
     */
    void setup() override {
      this->sensor->setup();
    };

    /**
     * Get the current memoized value
     */
    _Tp getValue() override {
      return this->value;
    };

    /**
     * Read actual value from the hardware and memoize it
     */
    void updateValue() {
      this->value = this->sensor->getValue();
    };
  };

  /**
   * Calibrated sensor decorator
   *
   * @tparam _Tp Type of the sensor value
   */
  template <typename _Tp>
  class CalibratedSensor : public ISensor<_Tp>, public Calibrated {
    protected:
      ISensor<_Tp>* sensor;
      Calibrator<_Tp>* calibrator;

    public:
      /**
       * @param sensor Sensor to be decorated
       * @param calibrator Calibrator algorithm to be used
       */
      CalibratedSensor(ISensor<_Tp>* sensor, Calibrator<_Tp>* calibrator) : sensor(sensor), calibrator(calibrator) {};

      _Tp getValue() override {
        auto value = this->sensor->getValue();

        if (this->calibrate) {
          this->calibrator->update(value);
        }

        return this->calibrator->calibrate(value);
      };

      void resetCalibration() override {
        this->calibrator->reset();
      };
  };
} // namespace OH
