#include "senseshift/arduino/components/serial_plotter.hpp"

namespace SenseShift::Arduino {
    struct PlaneVisitor {
        const SenseShift::Body::Haptics::Target target;
        HardwareSerial* serial;

        void operator()(const SenseShift::Body::Haptics::VibroPlane* plane) const
        {
            for (const auto& [position, state] : *(plane->getActuatorStates())) {
                this->serial->printf("Output[%u][%ux%u]:%u, ", this->target, position.x, position.y, state.intensity);
            }
        }
    };

    template<typename _Tp>
    void SerialPlotter_OutputStates<_Tp>::run()
    {
        while (true) {
            for (const auto& [target, plane] : *output->getTargets()) {
                std::visit(PlaneVisitor{ target, this->serial }, plane);
            }
            this->serial->println();

            delay(this->sampleRate);
        }
    }
} // namespace SenseShift::Arduino
