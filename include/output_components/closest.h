#include <output_component.hpp>

class ClosestOutputComponent : public OH::OutputComponent {
 protected:
  oh_output_point_t findClosestPoints(std::list<oh_output_point_t>& pts,
                                      oh_output_point_t& target);
  void setOutputs(oh_output_writers_map_t&);

 public:
  ClosestOutputComponent(oh_output_path_t path, oh_output_writers_map_t& outputs)
      : OutputComponent(path, outputs) {};
  ClosestOutputComponent(oh_output_path_t path, oh_output_writers_map_t& outputs, BaseType_t coreId)
      : OutputComponent(path, outputs, coreId) {};
  void writeOutput(oh_output_data_t&) override;
};
