#include <output_component.hpp>

class ClosestOutputComponent : public OH::OutputComponent {
 protected:
  oh_output_point_t findClosestPoints(std::list<oh_output_point_t>& pts,
                                      oh_output_point_t& target);
  void setOutputs(oh_output_writers_map_t&);

 public:
  ClosestOutputComponent(oh_output_writers_map_t& outputs)
      : OutputComponent(outputs){};
  void writeOutput(oh_output_data_t&);
};
