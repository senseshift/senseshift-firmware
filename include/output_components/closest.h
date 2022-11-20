#include "output_component.h"

class ClosestOutputComponent : OutputComponent
{
    public:
        ClosestOutputComponent(outputMap_t &outputs): OutputComponent(outputs) {};
        void setOutputs(std::map<outputPoint_t, OutputWriter*> &);
        void writeOutput(outputData_t&);
};