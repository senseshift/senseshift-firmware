#pragma once

#include <abstract_actuator.hpp>
#include <senseshift_interface.hpp>
#include <utility.hpp>

#include <list>
#include <set>
#include <map>
#include <vector>

namespace SenseShift {
    namespace Body {
        namespace Haptics {
            /**
             * Output "plane" (e.g. Chest, Palm, Finger, etc.)
             */
            class Plane {
              public:
                typedef SenseShift::Interface::Body::Haptics::Coordinate_t Coordinate_t;
                typedef SenseShift::Interface::Body::Haptics::Point_t Point_t;
                typedef SenseShift::Interface::Body::Haptics::Intensity_t Intensity_t;
                typedef std::map<Point_t, OH::AbstractActuator*> ActuatorMap_t;
                typedef std::map<Point_t, Intensity_t> StateMap_t;

              public:
                Plane() = default;
                Plane(ActuatorMap_t& outputs)
                {
                    this->setOutputs(outputs);
                };

                void setup();

                std::set<Point_t>* getOutputPoints()
                {
                    return &this->points;
                };
                StateMap_t* getOutputStates(void)
                {
                    return &this->states;
                };

                virtual void writeOutput(const Point_t&, const Intensity_t);

              protected:
                std::set<Point_t> points{};
                ActuatorMap_t writers{};
                StateMap_t states{};

                void setOutputs(ActuatorMap_t&);
            };

            class PlaneClosest : public Plane {
              public:
                PlaneClosest(ActuatorMap_t& outputs) : Plane(outputs){};
                void writeOutput(const Point_t&, const Intensity_t) override;
              protected:
                Point_t findClosestPoints(std::set<Point_t>&, const Point_t&);
            };

            /**
             * Maps a point index to output coordinate.
             * @todo: add per-axis margin settings with templates.
             */
            class PlaneMapper_Margin {
              public:
                /**
                 * Re-maps a point index to output coordinate.
                 * @tparam _Tp The type of the point index.
                 */
                template<typename _Tp>
                static inline Plane::Point_t mapPoint(_Tp x, _Tp y, _Tp x_max, _Tp y_max)
                {
                    const Plane::Coordinate_t x_coord = OH::simpleMap<_Tp>(x + 1, x_max + 2, OH_OUTPUT_COORD_MAX);
                    const Plane::Coordinate_t y_coord = OH::simpleMap<_Tp>(y + 1, y_max + 2, OH_OUTPUT_COORD_MAX);

                    return Plane::Point_t(x_coord, y_coord);
                }

                template<typename _Tp>
                static inline std::map<Plane::Point_t, _Tp*> mapMatrixCoordinates(std::vector<std::vector<_Tp*>> map2d)
                {
                    std::map<Plane::Point_t, _Tp*> points{};

                    size_t y_size = map2d.size();
                    size_t y_max = y_size - 1;

                    for (size_t y = 0; y < y_size; ++y) {
                        auto row = map2d.at(y);
                        size_t x_size = row.size();
                        size_t x_max = x_size - 1;

                        for (size_t x = 0; x < x_size; ++x) {
                            auto* wr = row.at(x);
                            Plane::Point_t coord = mapPoint(x, y, x_max, y_max);

                            points[coord] = wr;
                        }
                    }

                    return points;
                }
            };
        } // namespace Haptics
    } // namespace Body
} // namespace SenseShift
