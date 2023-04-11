#pragma once

#include <abstract_actuator.hpp>
#include <abstract_component.hpp>
#include <types.hpp>

#include <list>
#include <map>
#include <vector>

namespace OH {
  /**
   * Output "plane" (e.g. Chest, Palm, Finger, etc.)
   */
  class HapticPlane : public OH::IComponent {
   protected:
    std::list<oh_output_point_t> points{};
    oh_output_writers_map_t writers{};
    std::map<oh_output_point_t, oh_output_state_t> states{};

    void setOutputs(oh_output_writers_map_t&);

   public:
    HapticPlane(oh_output_writers_map_t& outputs) {
      this->setOutputs(outputs);
    };
    std::list<oh_output_point_t>* getOutputPoints(void) {
      return &this->points;
    };
    std::map<oh_output_point_t, oh_output_state_t>* getOutputStates(void) {
      return &this->states;
    };
    virtual void writeOutput(const oh_output_data_t&);
    void setup() override;
  };

  class HapticPlane_Closest : public HapticPlane {
   protected:
    oh_output_point_t findClosestPoints(std::list<oh_output_point_t>& pts, const oh_output_point_t& target);
    void setOutputs(oh_output_writers_map_t&);

   public:
    HapticPlane_Closest(oh_output_writers_map_t& outputs) : HapticPlane(outputs) {};
    void writeOutput(const oh_output_data_t&) override;
  };
}  // namespace OH
