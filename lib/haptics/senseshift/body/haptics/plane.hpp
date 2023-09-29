#pragma once

#include "senseshift/body/haptics/interface.hpp"

#include <senseshift/output/actuator.hpp>
#include <senseshift/utility.hpp>

#include <list>
#include <map>
#include <set>
#include <vector>

namespace SenseShift::Body::Haptics {
    using PositionSet = std::set<Position>;

    /**
     * Output "plane" (e.g. Chest, Palm, Finger, etc.)
     *
     * @tparam _Tp The type of the output value.
     */
    template<typename _Tp, typename _Ta>
    class ActuativePlane {
        static_assert(std::is_same<_Tp, VibroEffectData>());

      public:
        using Value = _Tp;
        using Actuator = _Ta;

        using ActuatorMap = std::map<Position, Actuator*>;
        using PositionStateMap = std::map<Position, Value>;

        ActuativePlane() = default;

        ActuativePlane(const ActuatorMap& actuators) { this->setActuators(actuators); }

        void setup();
        virtual void effect(const Position&, const Value&);

        [[nodiscard]] const PositionSet* getAvailablePoints() const { return &points; }
        [[nodiscard]] const PositionStateMap* getActuatorStates() const { return &states; }

      private:
        PositionSet points;
        ActuatorMap actuators{};
        PositionStateMap states{};

        void setActuators(const ActuatorMap&);
    };

    using VibroPlane = ActuativePlane<VibroEffectData, ::SenseShift::Output::IActuator<VibroEffectData::Intensity>>;

    /**
     * Output plane, finds the closest actuator for the given point.
     * @deprecated We should guarantee on the driver level, that the actuator is always exists
     */
    template<typename _Tp, typename _Ta>
    class ActuativePlane_Closest : public ActuativePlane<_Tp, _Ta> {
      public:
        using Value = _Tp;

        ActuativePlane_Closest(const typename ActuativePlane<_Tp, _Ta>::ActuatorMap& actuators) :
          ActuativePlane<_Tp, _Ta>(actuators)
        {
        }

        void effect(const Position&, const Value&) override;

      private:
        [[nodiscard]] const Position& findClosestPoint(const PositionSet&, const Position&) const;
    };

    using VibroPlane_Closest =
      ActuativePlane_Closest<VibroEffectData, ::SenseShift::Output::IActuator<VibroEffectData::Intensity>>;

    // TODO: configurable margin
    class PlaneMapper_Margin {
      public:
        template<typename _Tp>
        [[nodiscard]] static constexpr inline std::map<Position, _Tp*>
          mapMatrixCoordinates(std::vector<std::vector<_Tp*>> map2d)
        {
            std::map<Position, _Tp*> points{};

            size_t y_size = map2d.size();
            size_t y_max = y_size - 1;

            for (size_t y = 0; y < y_size; ++y) {
                auto row = map2d.at(y);
                size_t x_size = row.size();
                size_t x_max = x_size - 1;

                for (size_t x = 0; x < x_size; ++x) {
                    auto* wr = row.at(x);
                    Position coord = PlaneMapper_Margin::mapPoint<Position::Value>(x, y, x_max, y_max);

                    points[coord] = wr;
                }
            }

            return points;
        }

        /**
         * Re-maps a point index to output coordinate.
         * @tparam _Tp The type of the point index.
         */
        template<typename _Tp>
        [[nodiscard]] static constexpr inline ::SenseShift::Math::Point2<_Tp>
          mapPoint(_Tp x, _Tp y, _Tp x_max, _Tp y_max)
        {
            using Point = ::SenseShift::Math::Point2<_Tp>;

            return Point(
              ::SenseShift::accurateMap<_Tp>(x + 1, 0, x_max + 2, Point::MIN, Point::MAX),
              ::SenseShift::accurateMap<_Tp>(y + 1, 0, y_max + 2, Point::MIN, Point::MAX)
            );
        }
    };
} // namespace SenseShift::Body::Haptics
