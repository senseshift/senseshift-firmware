/**
 * Calibrated input
 * Credit: https://github.com/JohnRThomas/OpenGloves-Firmware/blob/main/open-gloves/Calibration.hpp
 */

#pragma once

#include <utility.hpp>

namespace OH {
    struct ICalibrated {
        virtual void resetCalibration() = 0;
        virtual void enableCalibration() = 0;
        virtual void disableCalibration() = 0;
    };

    class Calibrated : public virtual ICalibrated {
      protected:
        bool calibrate = false;

      public:
        virtual void resetCalibration() = 0;

        void enableCalibration() override
        {
            calibrate = true;
        }

        void disableCalibration() override
        {
            calibrate = false;
        }
    };

    template<typename _Tp>
    struct ICalibrator {
        virtual void reset() = 0;
        virtual void update(_Tp input) = 0;
        virtual _Tp calibrate(_Tp input) const = 0;
    };

    template<typename _Tp, _Tp output_min, _Tp output_max>
    class MinMaxCalibrator : public ICalibrator<_Tp> {
      public:
        MinMaxCalibrator() : value_min(output_max), value_max(output_min) {}

        void reset()
        {
            value_min = output_max;
            value_max = output_min;
        }

        void update(_Tp input)
        {
            // Update the min and the max.
            if (input < value_min)
                value_min = input;
            if (input > value_max)
                value_max = input;
        }

        _Tp calibrate(_Tp input) const
        {
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
            _Tp output = accurateMap<_Tp>(input, value_min, value_max, output_min, output_max);

            // Lock the range to the output.
            return std::clamp(output, output_min, output_max);
        }

      private:
        _Tp value_min;
        _Tp value_max;
    };

    template<typename _Tp, _Tp sensor_max, _Tp driver_max_deviation, _Tp output_min, _Tp output_max>
    class CenterPointDeviationCalibrator : public ICalibrator<_Tp> {
      public:
        CenterPointDeviationCalibrator() : range_min(sensor_max), range_max(0)
        {
#warning "CenterPointDeviationCalibrator is untested and may not work as expected."
        }

        void reset()
        {
            range_min = sensor_max;
            range_max = 0;
        }

        void update(_Tp input)
        {
            // Update the min and the max.
            if (input < range_min)
                range_min = accurateMap<_Tp>(input, output_min, output_max, 0, sensor_max);
            if (input > range_max)
                range_max = accurateMap<_Tp>(input, output_min, output_max, 0, sensor_max);
        }

        _Tp calibrate(_Tp input) const
        {
            // Find the center point of the sensor so we know how much we have deviated from it.
            _Tp center = (range_min + range_max) / 2.0f;

            // Map the input to the sensor range of motion.
            int output = accurateMap<_Tp>(input, output_min, output_max, 0, sensor_max);

            // Find the deviation from the center and clamp it to the maximum that the driver supports.
            output = std::clamp<int>(output - center, -driver_max_deviation, driver_max_deviation);

            // Finally map the deviation from the center back to the output range.
            return (_Tp) accurateMap<int>(output, -driver_max_deviation, driver_max_deviation, output_min, output_max);
        }

      private:
        _Tp range_min;
        _Tp range_max;
    };

    template<typename _Tp, _Tp sensor_max, _Tp driver_max_deviation, _Tp output_min, _Tp output_max>
    class FixedCenterPointDeviationCalibrator : public ICalibrator<_Tp> {
      public:
        void reset() {}
        void update(_Tp input) {}

        _Tp calibrate(_Tp input) const
        {
            // Find the center point of the sensor so we know how much we have deviated from it.
            _Tp center = sensor_max / 2.0f;

            // Map the input to the sensor range of motion.
            int output = accurateMap<_Tp>(input, output_min, output_max, 0, sensor_max);

            // Find the deviation from the center and clamp it to the maximum that the driver supports.
            output = std::clamp<int>(output - center, -driver_max_deviation, driver_max_deviation);

            // Finally map the deviation from the center back to the output range.
            return (_Tp) accurateMap<int>(output, -driver_max_deviation, driver_max_deviation, output_min, output_max);
        }
    };
} // namespace OH
