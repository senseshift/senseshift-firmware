#include "output_component.h"

class ClosestOutputComponent : public OutputComponent
{
    public:
        ClosestOutputComponent(outputMap_t &outputs): OutputComponent(outputs) {};
        void setOutputs(outputMap_t &);
        void writeOutput(outputData_t&);
};