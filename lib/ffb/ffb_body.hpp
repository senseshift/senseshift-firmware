#pragma once

#include <senseshift_interface.hpp>

#include <abstract_actuator.hpp>

#include <map>
#include <set>

namespace SenseShift {
    namespace Body {
        namespace ForceFeedback {
            class FFBBody
            {
              public:
                typedef SenseShift::Interface::Body::ForceFeedback::Joint Joint_t;
                typedef SenseShift::Interface::Body::ForceFeedback::Intensity_t Intensity_t;
                typedef std::map<Joint_t, OH::AbstractActuator*> ActuatorMap_t;

                FFBBody() = default;
                FFBBody(ActuatorMap_t& actuators)
                {
                    this->setActuators(actuators);
                }

                void setup();

                std::set<Joint_t>* getActiveActuators()
                {
                    return &activeActuators;
                }

                ActuatorMap_t* getActuators()
                {
                    return &actuators;
                }

                void handleMessage(const SenseShift::Interface::Body::ForceFeedback::Message&);

              private:
                ActuatorMap_t actuators;
                std::set<Joint_t> activeActuators;

                void setActuators(ActuatorMap_t&);
            };
        } // namespace ForceFeedback
    } // namespace Body
} // namespace SenseShift
