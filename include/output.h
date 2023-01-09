#pragma once

#include "config/all.h"

#include <abstract_component.hpp>
#include <output.hpp>
#include <output_component.hpp>

class Output {
 private:
  std::map<oh_output_path_t, OH::OutputComponent*> components;

 public:
  Output();

  void addComponent(oh_output_path_t, OH::OutputComponent*);
  std::map<oh_output_path_t, OH::OutputComponent*>* getComponents();

  void writeOutput(oh_output_path_t, oh_output_data_t&);
};
