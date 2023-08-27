#pragma once

#include "senseshift/body/skeleton/interface.hpp"

#include <senseshift/input/sensor.hpp>

namespace SenseShift::Body::Skeleton {
    class Skeleton {
      public:
        using Sensor_t = ::SenseShift::Input::IMemoizedSensor<JointValue_t>;
        typedef std::map<Joint_t, Sensor_t*> SensorMap_t;

        Skeleton() = default;

        void addSensor(const Joint_t joint, Sensor_t* sensor) { m_sensors[joint] = sensor; }

        const SensorMap_t& sensors() const { return m_sensors; }

        void setup()
        {
            for (auto& [joint, sensor] : m_sensors) {
                sensor->setup();
            }
        }

        void updateValues()
        {
            for (auto& [joint, sensor] : m_sensors) {
                sensor->updateValue();
            }
        }

      private:
        SensorMap_t m_sensors;
    };
} // namespace SenseShift::Body::Skeleton
