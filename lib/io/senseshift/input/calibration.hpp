/**
 * Calibrated input
 * Credit: https://github.com/JohnRThomas/OpenGloves-Firmware/blob/main/open-gloves/Calibration.hpp
 */

#pragma once

#include "senseshift/utility.hpp"

namespace SenseShift::Input {
    struct ICalibrated {
        virtual void startCalibration() = 0;
        virtual void stopCalibration() = 0;
        virtual void reselCalibration() = 0;
    };

    template<typename Tp>
    struct ICalibrator {
        /// Reset the calibration.
        virtual void reset() = 0;

        /// Update the calibration with a new input value.
        virtual void update(Tp input) = 0;

        /// Calibrate the input value.
        [[nodiscard]] virtual auto calibrate(Tp input) const -> Tp = 0;
    };

    template<typename Tp, Tp output_min, Tp output_max>
    class MinMaxCalibrator : public ICalibrator<Tp> {
        static_assert(std::is_arithmetic_v<Tp>, "MinMaxCalibrator only can be used with arithmetic types");

      public:
        using ValueType = Tp;

        MinMaxCalibrator() : value_min_(output_max), value_max_(output_min) {}

        void reset() override {
            value_min_ = output_max;
            value_max_ = output_min;
        }

        void update(ValueType input) override {
            // Update the min and the max.
            if (input < value_min_) {
                value_min_ = input;
            }
            if (input > value_max_) {
                value_max_ = input;
            }
        }

        auto calibrate(ValueType input) const -> ValueType override {
            // This means we haven't had any calibration data yet.
            // Return a neutral value right in the middle of the output range.
            if (value_min_ > value_max_) {
                return (output_min + output_max) / 2.0F;
            }

            if (input <= value_min_) {
                return output_min;
            }

            if (input >= value_max_) {
                return output_max;
            }

            // Map the input range to the output range.
            ValueType output = ::SenseShift::remap<ValueType, ValueType>(input, value_min_, value_max_, output_min, output_max);

            // Lock the range to the output.
            return std::clamp(output, output_min, output_max);
        }

      private:
        ValueType value_min_;
        ValueType value_max_;
    };

    template<typename Tp, Tp sensor_max, Tp driver_max_deviation, Tp output_min, Tp output_max>
    class CenterPointDeviationCalibrator : public ICalibrator<Tp> {
        static_assert(std::is_arithmetic_v<Tp>, "CenterPointDeviationCalibrator only can be used with arithmetic types");

      public:
        using ValueType = Tp;

        CenterPointDeviationCalibrator() : range_min(sensor_max), range_max(0) {}

        void reset() override {
            range_min = sensor_max;
            range_max = 0;
        }

        void update(Tp input) override {
            // Update the min and the max.
            if (input < range_min) {
                range_min = ::SenseShift::remap<ValueType>(input, output_min, output_max, 0, sensor_max);
            }
            if (input > range_max) {
                range_max = ::SenseShift::remap<ValueType>(input, output_min, output_max, 0, sensor_max);
            }
        }

        auto calibrate(ValueType input) const -> ValueType override {
            // Find the center point of the sensor, so we know how much we have deviated from it.
            Tp center = (range_min + range_max) / 2.0F;

            // Map the input to the sensor range of motion.
            int output = ::SenseShift::accurateMap<Tp>(input, output_min, output_max, 0, sensor_max);

            // Find the deviation from the center and clamp it to the maximum that the driver supports.
            output = std::clamp<int>(output - center, -driver_max_deviation, driver_max_deviation);

            // Finally map the deviation from the center back to the output range.
            return SenseShift::remap<ValueType, int>(
              output,
              -driver_max_deviation,
              driver_max_deviation,
              output_min,
              output_max
            );
        }

      private:
        Tp range_min;
        Tp range_max;
    };

    template<typename Tp, Tp sensor_max, Tp driver_max_deviation, Tp output_min, Tp output_max>
    class FixedCenterPointDeviationCalibrator : public ICalibrator<Tp> {
        static_assert(std::is_arithmetic_v<Tp>, "FixedCenterPointDeviationCalibrator only can be used with arithmetic types");

      public:
        using ValueType = Tp;

        void reset() override {}
        void update(ValueType input) override {}

        auto calibrate(ValueType input) const -> ValueType override {
            // Find the center point of the sensor, so we know how much we have deviated from it.
            Tp center = sensor_max / 2.0F;

            // Map the input to the sensor range of motion.
            int output = ::SenseShift::remap<int, Tp>(input, output_min, output_max, 0, sensor_max);

            // Find the deviation from the center and clamp it to the maximum that the driver supports.
            output = std::clamp<int>(output - center, -driver_max_deviation, driver_max_deviation);

            // Finally map the deviation from the center back to the output range.
            return SenseShift::remap<ValueType, int>(
              output,
              -driver_max_deviation,
              driver_max_deviation,
              output_min,
              output_max
            );
        }
    };
} // namespace SenseShift::Input
