#pragma once

#include "abstract_component.hpp"
#include "output_component.hpp"
#include "types.hpp"

#include <map>

namespace OH {
  // TODO: `IComponentRegistry<OutputComponent>` not working, need to
  // investigate
  typedef IComponentRegistry<AbstractComponent> app_registry_t;

  class Output {
   private:
    app_registry_t* app;
    std::map<oh_output_path_t, OutputComponent*> components{};

   public:
    Output(app_registry_t* app) : app(app){};

    void addComponent(oh_output_path_t, OutputComponent*);
    std::map<oh_output_path_t, OutputComponent*>* getComponents();

    void writeOutput(oh_output_path_t, oh_output_data_t&);
  };
}  // namespace OH
