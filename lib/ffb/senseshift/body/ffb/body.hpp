#pragma once

#include <senseshift/body/skeleton/interface.hpp>
#include <senseshift/output/actuator.hpp>

namespace SenseShift::Body::FFB {
    class FFBBody {
      public:
        using Actuator_t = ::SenseShift::Output::IActuator<Skeleton::JointValue_t>;
        typedef std::map<Skeleton::Joint_t, Actuator_t*> ActuatorMap_t;

        FFBBody() = default;

        void addActuator(Skeleton::Joint_t joint, Actuator_t* actuator) { m_actuators[joint] = actuator; }

        const ActuatorMap_t& actuators() const { return m_actuators; }

        void setJointState(const Skeleton::JointState_t& j) { setJointState(j.first, j.second); }

        void setJointState(const Skeleton::Joint_t joint, const Skeleton::JointValue_t value)
        {
            auto it = m_actuators.find(joint);
            if (it == m_actuators.end()) {
                log_w("Joint %d not found", joint);
            }

            it->second->writeOutput(value);
        }

      private:
        ActuatorMap_t m_actuators;
    };
} // namespace SenseShift::Body::FFB
