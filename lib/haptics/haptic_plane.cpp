#include "haptic_plane.hpp"

#include <logging.hpp>
#include <math.h>
#include <utility.hpp>

#include <algorithm>
#include <map>

namespace SenseShift {
    namespace Body {
        namespace Haptics {
            void Plane::writeOutput(const Point_t& point, const Intensity_t intensity)
            {
                if (this->points.find(point) == this->points.end()) {
                    log_w("No point found for (%d, %d)", point.x, point.y);
                    return;
                }

                this->states[point] = intensity;
                this->writers.at(point)->writeOutput(intensity);
            }

            void Plane::setOutputs(ActuatorMap_t& outputs)
            {
                this->writers.clear();
                this->writers = outputs;

                this->points.clear();
                for (auto& _p : outputs) {
                    this->points.insert(_p.first);
                }

                this->states.clear();
                for (auto& _p : outputs) {
                    this->states[_p.first] = {};
                }
            }

            void Plane::setup()
            {
                for (const auto& [point, actuator] : this->writers) {
                    actuator->setup();
                }
            }

            Plane::Point_t PlaneClosest::findClosestPoints(std::set<Point_t>& pts, const Point_t& target)
            {
                // find if coordinate is in set
                if (pts.find(target) != pts.end()) {
                    return target;
                }

                std::multimap<float, Point_t> mp = {};

                for (auto& _p : pts) {
                    float dx = abs(((float) target.x / OH_OUTPUT_COORD_MAX) - ((float) _p.x / OH_OUTPUT_COORD_MAX)),
                        dy = abs(((float) target.y / OH_OUTPUT_COORD_MAX) - ((float) _p.y / OH_OUTPUT_COORD_MAX));

                    auto dist = (float) sqrt(pow(dx, 2) + pow(dy, 2));

                    mp.insert({ dist, _p });
                }

                auto nearest = std::min_element(
                    mp.begin(),
                    mp.end(),
                    [](const std::pair<float, Point_t>& a, const std::pair<float, Point_t>& b) {
                        return a.first < b.first;
                    }
                );

                return nearest->second;
            }

            void PlaneClosest::writeOutput(const Point_t& point, const Intensity_t intensity)
            {
                const auto closestPoint = this->findClosestPoints(this->points, point);
                Plane::writeOutput(closestPoint, intensity);
            }
        } // namespace Haptics
    } // namespace Body
} // namespace SenseShift
