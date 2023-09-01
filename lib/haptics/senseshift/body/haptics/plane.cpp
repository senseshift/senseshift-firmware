#include "senseshift/body/haptics/plane.hpp"

#include <algorithm>
#include <cmath>
#include <senseshift/logging.hpp>

namespace SenseShift::Body::Haptics {
    template<typename _Tp, typename _Ta>
    void ActuativePlane<_Tp, _Ta>::setActuators(const ActuatorMap_t& actuators)
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
    void ActuativePlane<_Tp, _Ta>::effect(const Position_t& pos, const Value_t& val)
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
    void ActuativePlane_Closest<_Tp, _Ta>::effect(const Position_t& pos, const Value_t& val)
    {
        auto& closest = this->findClosestPoint(*this->getAvailablePoints(), pos);
        ActuativePlane<_Tp, _Ta>::effect(closest, val);
    }

    template<typename _Tp, typename _Ta>
    const Position_t&
      ActuativePlane_Closest<_Tp, _Ta>::findClosestPoint(const PositionSet_t& pts, const Position_t& target) const
    {
        // check if exact point exists
        auto it = pts.find(target);
        if (it != pts.end()) {
            return *it;
        }

        // find closest point by square distance
        std::multimap<float, Position_t> mp = {};
        for (const auto& _p : pts) {
            mp.insert({ (target - _p), _p });
        }

        auto nearest = std::min_element(mp.begin(), mp.end());

        return nearest->second;
    }

    template class ActuativePlane<VibroEffectData_t, ::SenseShift::Output::IActuator<VibroEffectData_t::Intensity_t>>;
    template class ActuativePlane_Closest<
      VibroEffectData_t,
      ::SenseShift::Output::IActuator<VibroEffectData_t::Intensity_t>>;
} // namespace SenseShift::Body::Haptics
