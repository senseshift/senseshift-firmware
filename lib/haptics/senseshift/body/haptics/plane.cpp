#include "senseshift/body/haptics/plane.hpp"
#include "senseshift/body/haptics/interface.hpp"

#include <algorithm>
#include <map>

#include <senseshift/output/output.hpp>
#include <senseshift/core/logging.hpp>

namespace SenseShift::Body::Haptics {
    static const char *const TAG = "haptic.plane";

    template<typename Tc, typename To>
    void OutputPlane<Tc, To>::setActuators(const ActuatorMap& actuators)
    {
        this->actuators_.clear();
        for (const auto& [point, actuator] : actuators) {
            this->actuators_[point] = actuator;
        }

        this->points_.clear();
        for (const auto& [point, _] : actuators) {
            this->points_.insert(point);
        }

        this->states_.clear();
        for (const auto& [point, _] : actuators) {
            this->states_[point] = static_cast<Value>(0);
        }
    }

    template<typename Tc, typename To>
    void OutputPlane<Tc, To>::setup()
    {
        for (const auto& [point, actuator] : this->actuators_) {
            actuator->init();
        }
    }

    template<typename Tc, typename To>
    void OutputPlane<Tc, To>::effect(const Position& pos, const Value& val)
    {
        auto find = this->actuators_.find(pos);
        if (find == this->actuators_.end()) {
            LOG_W(TAG, "No actuator for point (%u, %u)", pos.x, pos.y);
            return;
        }

        find->second->writeState(val);
        this->states_[pos] = val;
    }

    template<typename Tc, typename To>
    void OutputPlane_Closest<Tc, To>::effect(const Position& pos, const Value& val)
    {
        auto& closest = this->findClosestPoint(*this->getAvailablePoints(), pos);
        OutputPlane<Tc, To>::effect(closest, val);
    }

    template<typename Tc, typename To>
    [[nodiscard]] auto OutputPlane_Closest<Tc, To>::findClosestPoint(const PositionSet& pts, const Position& target) -> const Position&
    {
        // check if exact point exists
        const auto find = pts.find(target);
        if (find != pts.end()) {
            return *find;
        }

        // find the closest point by square distance
        std::multimap<float, Position> distance_map = {};
        for (const auto& point : pts) {
            distance_map.insert({(target - point), point });
        }

        const auto nearest = std::min_element(distance_map.begin(), distance_map.end());

        return nearest->second;
    }

    template class OutputPlane<Position::Value, Output::FloatOutput::ValueType>;
    template class OutputPlane_Closest<Position::Value, Output::FloatOutput::ValueType>;
} // namespace SenseShift::Body::Haptics
