#include "output_component.h"

class ClosestOutputComponent : OutputComponent
{
    public:
        void setOutputs(std::map<outputPoint_t, OutputWriter*> &);
        void writeOutput(outputData_t&);
};