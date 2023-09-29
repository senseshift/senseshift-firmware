#include "senseshift/body/haptics/plane.hpp"

#include <algorithm>
#include <cmath>
#include <senseshift/logging.hpp>

namespace SenseShift::Body::Haptics {
    template<typename _Tp, typename _Ta>
    void ActuativePlane<_Tp, _Ta>::setActuators(const ActuatorMap& actuators)
    {
        this->actuators.clear();
        for (const auto& [point, actuator] : actuators) {
            this->actuators[point] = actuator;
        }

        this->points.clear();
        for (const auto& [point, _] : actuators) {
            this->points.insert(point);
        }

        this->states.clear();
        for (const auto& [point, _] : actuators) {
            this->states[point] = 0;
        }
    }

    template<typename _Tp, typename _Ta>
    void ActuativePlane<_Tp, _Ta>::setup()
    {
        for (const auto& [point, actuator] : this->actuators) {
            actuator->setup();
        }
    }

    template<typename _Tp, typename _Ta>
    void ActuativePlane<_Tp, _Ta>::effect(const Position& pos, const Value& val)
    {
        auto it = this->actuators.find(pos);
        if (it == this->actuators.end()) {
            log_w("No actuator for point (%u, %u)", pos.x, pos.y);
            return;
        }

        it->second->writeOutput(val);
        this->states[pos] = val;
    }

    template<typename _Tp, typename _Ta>
    void ActuativePlane_Closest<_Tp, _Ta>::effect(const Position& pos, const Value& val)
    {
        auto& closest = this->findClosestPoint(*this->getAvailablePoints(), pos);
        ActuativePlane<_Tp, _Ta>::effect(closest, val);
    }

    template<typename _Tp, typename _Ta>
    const Position&
      ActuativePlane_Closest<_Tp, _Ta>::findClosestPoint(const PositionSet& pts, const Position& target) const
    {
        // check if exact point exists
        auto it = pts.find(target);
        if (it != pts.end()) {
            return *it;
        }

        // find closest point by square distance
        std::multimap<float, Position> mp = {};
        for (const auto& _p : pts) {
            mp.insert({ (target - _p), _p });
        }

        auto nearest = std::min_element(mp.begin(), mp.end());

        return nearest->second;
    }

    template class ActuativePlane<VibroEffectData, ::SenseShift::Output::IActuator<VibroEffectData::Intensity>>;
    template class ActuativePlane_Closest<VibroEffectData, ::SenseShift::Output::IActuator<VibroEffectData::Intensity>>;
} // namespace SenseShift::Body::Haptics
