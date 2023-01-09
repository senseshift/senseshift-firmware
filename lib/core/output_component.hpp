#pragma once

#include <abstract_component.hpp>
#include <abstract_output_writer.hpp>
#include <output.hpp>
#include <vec2.hpp>

#include <list>
#include <map>
#include <vector>

namespace OH
{
  //! Output "plane" (e.g. Chest, Palm, Finger, etc.)
  class OutputComponent : public OH::AbstractComponent {
   protected:
    std::list<oh_output_point_t> points{};
    oh_output_writers_map_t writers{};
    std::map<oh_output_point_t, oh_output_state_t> states{};

    void setOutputs(oh_output_writers_map_t&);

   public:
    OutputComponent(oh_output_writers_map_t& outputs) { this->setOutputs(outputs); };
    std::list<oh_output_point_t>* getOutputPoints(void) { return &this->points; };
    std::map<oh_output_point_t, oh_output_state_t>* getOutputStates(void) {
      return &this->states;
    };
    virtual void writeOutput(oh_output_data_t&) = 0;
    void setup() override;
  };
} // namespace OH
