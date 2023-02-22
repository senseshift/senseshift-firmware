#pragma once

#include "abstract_component.hpp"
#include "abstract_output_writer.hpp"
#include "types.hpp"

#include <list>
#include <map>
#include <vector>

namespace OH {
  //! Output "plane" (e.g. Chest, Palm, Finger, etc.)
  class OutputComponent : public OH::AbstractComponent {
   private:
    oh_output_path_t path;
   protected:
    std::list<oh_output_point_t> points{};
    oh_output_writers_map_t writers{};
    std::map<oh_output_point_t, oh_output_state_t> states{};

    void setOutputs(oh_output_writers_map_t&);

   public:
    OutputComponent(oh_output_path_t path, oh_output_writers_map_t& outputs, const BaseType_t coreId) : OH::AbstractComponent({ "OutputComponent", 4096, 1, coreId }), path(path) {
      this->setOutputs(outputs);
    };
    oh_output_path_t getPath(void) {
      return this->path;
    };
    std::list<oh_output_point_t>* getOutputPoints(void) {
      return &this->points;
    };
    std::map<oh_output_point_t, oh_output_state_t>* getOutputStates(void) {
      return &this->states;
    };
    virtual void writeOutput(oh_output_data_t&);
    void setup() override;
  };
}  // namespace OH
