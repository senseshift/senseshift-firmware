#include "ffb_body.hpp"

namespace SenseShift::Body::ForceFeedback {
    void FFBBody::setup()
    {
        for (auto& [joint, actuator] : this->actuators) {
            actuator->setup();
        }
    }

    void FFBBody::setActuators(ActuatorMap_t& actuators)
    {
        this->actuators.clear();
        this->actuators = actuators;

        this->activeActuators.clear();
        for (auto& [joint, actuator] : this->actuators) {
            this->activeActuators.insert(joint);
        }
    }

    void FFBBody::handleMessage(const SenseShift::Interface::Body::ForceFeedback::Message& message)
    {
        if (this->activeActuators.find(message.joint) != this->activeActuators.end()) {
            this->actuators[message.joint]->writeOutput(message.intensity);
        }
    }
}
