#pragma once

#include "haptics_interface.hpp"

#include <abstract_actuator.hpp>
#include <types.hpp>
#include <utility.hpp>

#include <list>
#include <map>
#include <set>
#include <vector>

namespace SenseShift::Body::Haptics {
    typedef std::set<Position_t> PositionSet_t;

    /**
     * Output "plane" (e.g. Chest, Palm, Finger, etc.)
     *
     * @tparam _Tp The type of the output value.
     */
    template<typename _Tp>
    class ActuativePlane {
        static_assert(std::is_same<_Tp, VibroEffectData_t>());

      public:
        typedef _Tp Value_t;
        typedef std::map<Position_t, Value_t> PositionStateMap_t;

        /**
         * The type of the actuator.
         * @TODO: Make this a template parameter
         */
        typedef OH::AbstractActuator Actuator_t;
        typedef std::map<Position_t, Actuator_t*> ActuatorMap_t;

        ActuativePlane(const ActuatorMap_t& actuators)
        {
            this->setActuators(actuators);
        }

        void setup();
        virtual void effect(const Position_t&, const Value_t&);

        [[nodiscard]] const PositionSet_t* getAvailablePoints() const
        {
            return &points;
        }
        [[nodiscard]] const PositionStateMap_t* getActuatorStates() const
        {
            return &states;
        }

      private:
        PositionSet_t points;
        ActuatorMap_t actuators{};
        PositionStateMap_t states{};

        void setActuators(const ActuatorMap_t&);
    };

    typedef ActuativePlane<VibroEffectData_t> VibroPlane;

    /**
     * Output plane, finds the closest actuator for the given point.
     * @deprecated We should guarantee on the driver level, that the actuator is always exists
     */
    template<typename _Tp>
    class ActuativePlane_Closest : public ActuativePlane<_Tp> {
      public:
        typedef _Tp Value_t;

        ActuativePlane_Closest(const typename ActuativePlane<_Tp>::ActuatorMap_t& actuators) :
          ActuativePlane<_Tp>(actuators)
        {
        }

        void effect(const Position_t&, const Value_t&) override;

      private:
        [[nodiscard]] const Position_t& findClosestPoint(const PositionSet_t&, const Position_t&) const;
    };

    typedef ActuativePlane_Closest<VibroEffectData_t> VibroPlane_Closest;

    // TODO: configurable margin
    class PlaneMapper_Margin {
      public:
        template<typename _Tp>
        [[nodiscard]] static constexpr inline std::map<oh_output_point_t, _Tp*>
          mapMatrixCoordinates(std::vector<std::vector<_Tp*>> map2d)
        {
            std::map<Position_t, _Tp*> points{};

            size_t y_size = map2d.size();
            size_t y_max = y_size - 1;

            for (size_t y = 0; y < y_size; ++y) {
                auto row = map2d.at(y);
                size_t x_size = row.size();
                size_t x_max = x_size - 1;

                for (size_t x = 0; x < x_size; ++x) {
                    auto* wr = row.at(x);
                    Position_t coord = PlaneMapper_Margin::mapPoint<Position_t::Value_t>(x, y, x_max, y_max);

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
        [[nodiscard]] static constexpr inline OH::Point2<_Tp> mapPoint(_Tp x, _Tp y, _Tp x_max, _Tp y_max)
        {
            using Point_t = OH::Point2<_Tp>;
            return Point_t(
              OH::accurateMap<_Tp>(x + 1, 0, x_max + 2, Point_t::MIN, Point_t::MAX),
              OH::accurateMap<_Tp>(y + 1, 0, y_max + 2, Point_t::MIN, Point_t::MAX)
            );
        }
    };
} // namespace SenseShift::Body::Haptics
