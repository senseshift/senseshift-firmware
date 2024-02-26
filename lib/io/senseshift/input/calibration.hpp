/// Calibrated input
/// Credit: https://github.com/JohnRThomas/OpenGloves-Firmware/blob/main/open-gloves/Calibration.hpp

#pragma once

#include "senseshift/utility.hpp"

#include <type_traits>

namespace SenseShift::Input::Calibration {
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

    template<typename Tp>
    class MinMaxCalibrator : public ICalibrator<Tp> {
        static_assert(std::is_arithmetic_v<Tp>, "MinMaxCalibrator only can be used with arithmetic types");

      public:
        using ValueType = Tp;

        explicit MinMaxCalibrator(Tp output_min, Tp output_max) :
          output_min_(output_min), output_max_(output_max), value_min_(output_max), value_max_(output_min)
        {
        }

        template<typename U = Tp, std::enable_if_t<std::is_same_v<U, float>, int> = 0>
        explicit MinMaxCalibrator(Tp output_min = 0.0F, Tp output_max = 1.0F) :
          output_min_(output_min), output_max_(output_max), value_min_(output_max), value_max_(output_min)
        {
        }

        void reset() override
        {
            value_min_ = output_max_;
            value_max_ = output_min_;
        }

        void update(ValueType input) override
        {
            // Update the min and the max.
            if (input < value_min_) {
                value_min_ = input;
            }
            if (input > value_max_) {
                value_max_ = input;
            }
        }

        auto calibrate(ValueType input) const -> ValueType override
        {
            // This means we haven't had any calibration data yet.
            // Return a neutral value right in the middle of the output range.
            if (value_min_ > value_max_) {
                return (output_min_ + output_max_) / 2.0F;
            }

            if (input <= value_min_) {
                return output_min_;
            }

            if (input >= value_max_) {
                return output_max_;
            }

            // Map the input range to the output range.
            ValueType output =
              ::SenseShift::remap<ValueType, ValueType>(input, value_min_, value_max_, output_min_, output_max_);

            // Lock the range to the output.
            return std::clamp(output, output_min_, output_max_);
        }

      private:
        const ValueType output_min_;
        const ValueType output_max_;

        ValueType value_min_;
        ValueType value_max_;
    };

    template<typename Tp>
    class CenterPointDeviationCalibrator : public ICalibrator<Tp> {
        static_assert(
          std::is_arithmetic_v<Tp>, "CenterPointDeviationCalibrator only can be used with arithmetic types"
        );

      public:
        using ValueType = Tp;

        CenterPointDeviationCalibrator(Tp sensor_max, Tp driver_max_deviation, Tp output_min, Tp output_max) :
          sensor_max_(sensor_max),
          driver_max_deviation_(driver_max_deviation),
          output_min_(output_min),
          output_max_(output_max),
          range_min_(sensor_max),
          range_max_(0)
        {
        }

        template<typename U = Tp, std::enable_if_t<std::is_same_v<U, float>, int> = 0>
        CenterPointDeviationCalibrator(
          Tp sensor_max, Tp driver_max_deviation, Tp output_min = 0.0F, Tp output_max = 1.0F
        ) :
          sensor_max_(sensor_max),
          driver_max_deviation_(driver_max_deviation),
          output_min_(output_min),
          output_max_(output_max),
          range_min_(sensor_max),
          range_max_(0)
        {
        }

        void reset() override
        {
            this->range_min_ = this->sensor_max_;
            this->range_max_ = 0;
        }

        void update(Tp input) override
        {
            // Update the min and the max.
            if (input < this->range_min_) {
                this->range_min_ =
                  ::SenseShift::remap<ValueType>(input, this->output_min_, this->output_max_, 0, this->sensor_max_);
            }
            if (input > this->range_max_) {
                this->range_max_ =
                  ::SenseShift::remap<ValueType>(input, this->output_min_, this->output_max_, 0, this->sensor_max_);
            }
        }

        auto calibrate(ValueType input) const -> ValueType override
        {
            // Find the center point of the sensor, so we know how much we have deviated from it.
            Tp center = (this->range_min_ + this->range_max_) / 2.0F;

            // Map the input to the sensor range of motion.
            int output = ::SenseShift::remap<Tp>(input, this->output_min_, this->output_max_, 0, this->sensor_max_);

            // Find the deviation from the center and clamp it to the maximum that the driver supports.
            output = std::clamp<int>(output - center, -(this->driver_max_deviation_), this->driver_max_deviation_);

            // Finally map the deviation from the center back to the output range.
            return SenseShift::remap<ValueType, int>(
              output,
              -(this->driver_max_deviation_),
              this->driver_max_deviation_,
              this->output_min_,
              this->output_max_
            );
        }

      private:
        const Tp sensor_max_;
        const Tp driver_max_deviation_;
        const Tp output_min_;
        const Tp output_max_;

        Tp range_min_;
        Tp range_max_;
    };

    template<typename Tp>
    class FixedCenterPointDeviationCalibrator : public ICalibrator<Tp> {
        static_assert(
          std::is_arithmetic_v<Tp>, "FixedCenterPointDeviationCalibrator only can be used with arithmetic types"
        );

      public:
        using ValueType = Tp;

        explicit FixedCenterPointDeviationCalibrator(
          Tp sensor_max, Tp driver_max_deviation, Tp output_min, Tp output_max
        ) :
          sensor_max_(sensor_max),
          driver_max_deviation_(driver_max_deviation),
          output_min_(output_min),
          output_max_(output_max)
        {
        }

        template<typename U = Tp, std::enable_if_t<std::is_same_v<U, float>, int> = 0>
        explicit FixedCenterPointDeviationCalibrator(
          Tp sensor_max, Tp driver_max_deviation, Tp output_min = 0.0F, Tp output_max = 1.0F
        ) :
          sensor_max_(sensor_max),
          driver_max_deviation_(driver_max_deviation),
          output_min_(output_min),
          output_max_(output_max)
        {
        }

        void reset() override {}
        void update(ValueType input) override {}

        auto calibrate(ValueType input) const -> ValueType override
        {
            // Find the center point of the sensor, so we know how much we have deviated from it.
            Tp center = this->sensor_max_ / 2.0F;

            // Map the input to the sensor range of motion.
            int output = ::SenseShift::remap<Tp>(input, this->output_min_, this->output_max_, 0, this->sensor_max_);

            // Find the deviation from the center and clamp it to the maximum that the driver supports.
            output = std::clamp<int>(output - center, -(this->driver_max_deviation_), this->driver_max_deviation_);

            // Finally map the deviation from the center back to the output range.
            return SenseShift::remap<ValueType, int>(
              output,
              -(this->driver_max_deviation_),
              this->driver_max_deviation_,
              this->output_min_,
              this->output_max_
            );
        }

      private:
        const Tp sensor_max_;
        const Tp driver_max_deviation_;
        const Tp output_min_;
        const Tp output_max_;
    };
} // namespace SenseShift::Input::Calibration
