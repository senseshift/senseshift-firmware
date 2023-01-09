#include "output_component.h"

class ClosestOutputComponent : public OutputComponent {
 protected:
  outputPoint_t findClosestPoints(std::list<outputPoint_t>& pts,
                                  outputPoint_t& target);
  void setOutputs(outputMap_t&);

 public:
  ClosestOutputComponent(outputMap_t& outputs) : OutputComponent(outputs){};
  void writeOutput(outputData_t&);
};
