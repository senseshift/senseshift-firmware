#pragma once

#include "output_constants.h"

#include "abstract_component.hpp"
#include "output_component.hpp"
#include "types.hpp"

#include <map>

namespace OH {
  // TODO: `IComponentRegistry<OutputComponent>` not working, need to
  // investigate
  typedef IComponentRegistry<IComponent> app_registry_t;

  class Output {
   private:
    app_registry_t* app;
    std::map<oh_output_path_t, OutputComponent*> components{};

   public:
    Output(app_registry_t* app) : app(app){};

    void addComponent(OutputComponent*);
    std::map<oh_output_path_t, OutputComponent*>* getComponents();

    void writeOutput(const oh_output_path_t, const oh_output_data_t&);
  };

  /**
   * Re-maps a point index to output coordinate.
   * @tparam _Tp The type of the point index.
   */
  template <typename _Tp>
  oh_output_point_t* mapPoint(_Tp x, _Tp y, _Tp x_max, _Tp y_max) {
    const oh_output_coord_t x_coord =
        static_cast<_Tp>(1 == x_max ? 1 : OH_OUTPUT_COORD_MAX * (1 / (float)x_max) * ((float)x));
    const oh_output_coord_t y_coord =
        static_cast<_Tp>(1 == y_max ? 1 : OH_OUTPUT_COORD_MAX * (1 / (float)y_max) * ((float)y));

    return new oh_output_point_t(x_coord, y_coord);
  }

  template <typename _Tp>
  std::map<oh_output_point_t, _Tp*> mapMatrixCoordinates(std::vector<std::vector<_Tp*>> map2d);
}  // namespace OH
