#pragma once

#include "abstract_output_writer.hpp"

#include <abstract_component.hpp>
#include <types.hpp>

#include <list>
#include <map>
#include <vector>

namespace OH {
  /**
   * Output "plane" (e.g. Chest, Palm, Finger, etc.)
   */
  class OutputComponent : public OH::AbstractComponent {
   private:
    oh_output_path_t path;
   protected:
    std::list<oh_output_point_t> points{};
    oh_output_writers_map_t writers{};
    std::map<oh_output_point_t, oh_output_state_t> states{};

    void setOutputs(oh_output_writers_map_t&);

   public:
    OutputComponent(oh_output_path_t path, oh_output_writers_map_t& outputs) : path(path) {
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
    virtual void writeOutput(const oh_output_data_t&);
    void setup() override;
  };

  class ClosestOutputComponent : public OutputComponent {
   protected:
    oh_output_point_t findClosestPoints(std::list<oh_output_point_t>& pts, const oh_output_point_t& target);
    void setOutputs(oh_output_writers_map_t&);

   public:
    ClosestOutputComponent(oh_output_path_t path, oh_output_writers_map_t& outputs)
        : OutputComponent(path, outputs) {};
    void writeOutput(const oh_output_data_t&) override;
  };
}  // namespace OH
