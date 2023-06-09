/**
 * Calibrated input
 * Credit: https://github.com/JohnRThomas/OpenGloves-Firmware/blob/main/open-gloves/Calibration.hpp
 */

#pragma once

#include <utility.hpp>

namespace OH {
  class Calibrated {
    protected:
      bool calibrate = false;

    public:
      virtual void resetCalibration() = 0;

      void enableCalibration() {
        calibrate = true;
      }

      void disableCalibration() {
        calibrate = false;
      }
  };

  template<typename T>
  struct Calibrator {
    virtual void reset() = 0;
    virtual void update(T input) = 0;
    virtual T calibrate(T input) const = 0;
  };

  template<typename T, T output_min, T output_max>
  class MinMaxCalibrator : public Calibrator<T> {
  public:
    MinMaxCalibrator() : value_min(output_max), value_max(output_min) {}

    void reset() {
      value_min = output_max;
      value_max = output_min;
    }

    void update(T input) {
      // Update the min and the max.
      if (input < value_min) value_min = input;
      if (input > value_max) value_max = input;
    }

    T calibrate(T input) const {
      // This means we haven't had any calibration data yet.
      // Return a neutral value right in the middle of the output range.
      if (value_min > value_max) {
        return (output_min + output_max) / 2.0f;
      }

      if (input <= value_min) {
        return output_min;
      }

      if (input >= value_max) {
        return output_max;
      }

      // Map the input range to the output range.
      T output = accurateMap(input, value_min, value_max, output_min, output_max);

      // Lock the range to the output.
      return constrain(output, output_min, output_max);
    }

  private:
    T value_min;
    T value_max;
  };

  template<typename T, T sensor_max, T driver_max_deviation, T output_min, T output_max>
  class CenterPointDeviationCalibrator : public Calibrator<T> {
  public:
    CenterPointDeviationCalibrator() : range_min(sensor_max), range_max(0) {}

    void reset() {
      range_min = sensor_max;
      range_max = 0;
    }

    void update(T input) {
      // Update the min and the max.
      if (input < range_min) range_min = accurateMap(input, output_min, output_max, 0, sensor_max);
      if (input > range_max) range_max = accurateMap(input, output_min, output_max, 0, sensor_max);
    }

    T calibrate(T input) const {
      // Find the center point of the sensor so we know how much we have deviated from it.
      T center = (range_min + range_max) / 2.0f;

      // Map the input to the sensor range of motion.
      T output = accurateMap(input, output_min, output_max, 0, sensor_max);

      // Find the deviation from the center and constrain it to the maximum that the driver supports.
      output = constrain(output - center, -driver_max_deviation, driver_max_deviation);

      // Finally map the deviation from the center back to the output range.
      return accurateMap(output, -driver_max_deviation, driver_max_deviation, output_min, output_max);
    }

  private:
    T range_min;
    T range_max;
  };

  template<typename T, T sensor_max, T driver_max_deviation, T output_min, T output_max>
  class FixedCenterPointDeviationCalibrator : public Calibrator<T> {
  public:
    void reset() {}
    void update(T input) {}

    T calibrate(T input) const {
      // Find the center point of the sensor so we know how much we have deviated from it.
      T center = sensor_max / 2.0f;

      // Map the input to the sensor range of motion.
      T output = accurateMap(input, output_min, output_max, 0, sensor_max);

      // Find the deviation from the center and constrain it to the maximum that the driver supports.
      output = constrain(output - center, -driver_max_deviation, driver_max_deviation);

      // Finally map the deviation from the center back to the output range.
      return accurateMap(output, -driver_max_deviation, driver_max_deviation, output_min, output_max);
    }
  };
}
