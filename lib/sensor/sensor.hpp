#pragma once

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
} // namespace OH
