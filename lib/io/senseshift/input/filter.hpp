#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <functional>
#include <type_traits>
#include <utility>
#include <vector>

#include <senseshift/core/helpers.hpp>
#include <senseshift/core/logging.hpp>

namespace SenseShift::Input::Filter {
template<typename Tp>
class ISimpleSensor {
  public:
    using ValueType = Tp;

    virtual auto getValue() -> ValueType = 0;
};

template<typename Tp>
class Sensor;

template<typename Tp>
class IFilter {
  public:
    using ValueType = Tp;

    virtual auto filter(ISimpleSensor<ValueType>* sensor, ValueType value) -> ValueType = 0;
};

template<typename Tp>
class Filtered {
  public:
    using ValueType = Tp;
    using FilterType = IFilter<ValueType>*;

    auto getFilters() -> std::vector<FilterType>&
    {
        return this->filters_;
    }

    /// Appends a filter to the sensor's filter chain.
    ///
    /// \param filter The filter to add.
    ///
    /// \see addFilters for adding multiple filters.
    void addFilter(const FilterType& filter)
    {
        this->filters_.push_back(filter);
    }

    /// Adds multiple filters to the sensor's filter chain. Appends to the end of the chain.
    ///
    /// \param filters The chain of filters to add.
    ///
    /// \example
    /// \code
    /// sensor->addFilters({
    ///     new MinMaxFilter(0.1f, 0.9f),
    ///     new CenterDeadzoneFilter(0.1f),
    /// });
    /// \endcode
    ///
    /// todo(leon0399): use SFINAE
    void addFilters(const std::vector<FilterType>& filters)
    {
        this->filters_.insert(this->filters_.end(), filters.begin(), filters.end());
    }

    /// Replaces the sensor's filter chain with the given filters.
    ///
    /// \param filters New filter chain.
    ///
    /// \example
    /// \code
    /// sensor->setFilters({
    ///     new MinMaxFilter(0.1f, 0.9f),
    ///     new CenterDeadzoneFilter(0.1f),
    /// });
    /// \endcode
    void setFilters(const std::vector<FilterType>& filters)
    {
        this->filters_ = filters;
    }

    /// Removes everything from the sensor's filter chain.
    void clearFilters()
    {
        this->filters_.clear();
    }

  protected:
    std::vector<FilterType> filters_ = {};
};

template<typename Tp>
class AddFilter : public IFilter<Tp> {
  public:
    explicit AddFilter(Tp offset) : offset_(offset)
    {
    }

    auto filter(ISimpleSensor<Tp>* /*sensor*/, Tp value) -> Tp override
    {
        return value + this->offset_;
    }

  private:
    Tp offset_;
};

template<typename Tp>
class SubtractFilter : public IFilter<Tp> {
  public:
    explicit SubtractFilter(Tp offset) : offset_(offset)
    {
    }

    auto filter(ISimpleSensor<Tp>* /*sensor*/, Tp value) -> Tp override
    {
        return value - this->offset_;
    }

  private:
    Tp offset_;
};

template<typename Tp>
class MultiplyFilter : public IFilter<Tp> {
  public:
    explicit MultiplyFilter(Tp factor) : factor_(factor){};

    auto filter(ISimpleSensor<Tp>* /*sensor*/, Tp value) -> Tp override
    {
        return value * this->factor_;
    }

  private:
    Tp factor_;
};

/// Calculates the original voltage from the voltage divider.
///
/// \param r1 The resistance in Ohms of the first resistor in the voltage divider.
/// Example: 27000.0F.
/// \param r2 The resistance in Ohms of the second resistor in the voltage divider.
/// Example: 100000.0F.
///
/// \example
/// \code
/// new VoltageDividerFilter(27000.0F, 100000.0F);
/// \endcode
#define VoltageDividerFilter(r1, r2) MultiplyFilter((r1 + r2) / r2)

template<typename Tp>
class ClampFilter : public IFilter<Tp> {
  public:
    ClampFilter(Tp min, Tp max) : min_(min), max_(max){};

    auto filter(ISimpleSensor<Tp>* /*source*/, Tp value) -> Tp override
    {
        return std::clamp(value, this->min_, this->max_);
    }

  private:
    Tp min_;
    Tp max_;
};

/// An alias for ClampFilter.
template<typename Tp>
using MinMaxFilter = ClampFilter<Tp>;

/// An alias for ClampFilter.
template<typename Tp>
using RangeFilter = ClampFilter<Tp>;

template<typename Tp>
class LambdaFilter : public IFilter<Tp> {
  public:
    using Lambda = std::function<Tp(Tp)>;

    explicit LambdaFilter(Lambda filter) : filter_(std::move(filter)){};

    auto filter(ISimpleSensor<Tp>* /*sensor*/, Tp value) -> Tp override
    {
        return this->filter_(value);
    }

  private:
    Lambda filter_;
};

template<typename Tp>
class SlidingWindowMovingAverageFilter : public IFilter<Tp> {
    static_assert(std::is_arithmetic_v<Tp>, "SlidingWindowAverageFilter only supports arithmetic types");

  public:
    explicit SlidingWindowMovingAverageFilter(size_t window_size) : window_size_(window_size){};

    auto filter(ISimpleSensor<Tp>* /*sensor*/, Tp value) -> Tp override
    {
        while (this->queue_.size() >= this->window_size_) {
            this->queue_.pop_front();
        }
        this->queue_.push_back(value);

        return this->getAverage();
    }

  private:
    std::size_t window_size_;
    std::deque<Tp> queue_;

    [[nodiscard]] auto getAverage() const -> Tp
    {
        Tp sum = Tp();
        for (auto value : this->queue_) {
            // cppcheck-suppress useStlAlgorithm; We may run in no-stl environment
            sum += value;
        }
        return sum / this->queue_.size();
    }
};

template<typename Tp>
class ExponentialMovingAverageFilter : public IFilter<Tp> {
    static_assert(std::is_arithmetic_v<Tp>, "ExponentialMovingAverageFilter only supports arithmetic types");

  public:
    explicit ExponentialMovingAverageFilter(float alpha) : alpha_(alpha){};

    auto filter(ISimpleSensor<Tp>* /*sensor*/, Tp value) -> Tp override
    {
        if (this->is_first_) {
            this->is_first_ = false;

            this->acc_ = value;
            return this->acc_;
        }

        this->acc_ = (this->alpha_ * value) + ((1 - this->alpha_) * this->acc_);
        return this->acc_;
    }

  private:
    bool is_first_ = true;
    float alpha_;
    Tp acc_;
};

/// Deadzone filter. Clamps acc_ to center if it is within the deadzone.
/// Usually used to filter out noise in the joystick.
class SinglePointDeadzoneFilter : public IFilter<float> {
  public:
    explicit SinglePointDeadzoneFilter(float deadzone, float center = 0.5F) : deadzone_(deadzone), center_(center){};

    inline auto filter(ISimpleSensor<float>* /*sensor*/, float value) -> float override
    {
        const auto deviation = std::abs(value - this->center_);
        return deviation < deadzone_ ? this->center_ : value;
    }

  private:
    const float deadzone_;
    const float center_;
};

using CenterDeadzoneFilter = SinglePointDeadzoneFilter;

/// Interpolates the value from the lookup table.
/// Can be used to determine battery level from the voltage.
///
/// \tparam Tp Type of the lookup table values.
/// \tparam Container Type of the lookup table container.
template<typename Container, typename Tp = typename Container::mapped_type>
class LookupTableInterpolationFilter : public IFilter<Tp> {
    static_assert(std::is_same_v<typename Container::key_type, Tp>);
    static_assert(std::is_arithmetic_v<Tp>, "LookupTableInterpolationFilter only supports arithmetic types");

  public:
    explicit LookupTableInterpolationFilter(Container const& lookup_table) : lookup_table_(lookup_table){};

    auto filter(ISimpleSensor<float>* /*sensor*/, Tp value) -> Tp override
    {
        return SenseShift::lookup_table_interpolate_linear<Container, Tp, Tp>(this->lookup_table_, value);
    }

  private:
    Container const& lookup_table_;
};

/// Specialized filter for analog sensors (between 0.0 and 1.0).
class AnalogInvertFilter : public IFilter<float> {
  public:
    auto filter(ISimpleSensor<float>* /*sensor*/, float value) -> float override
    {
        return 1.0F - value;
    }
};
} // namespace SenseShift::Input::Filter
