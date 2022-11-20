#pragma once

#include "config/output.h"

#include "component.h"
#include "output_component.h"

class Output
{
    private:
        std::map<outputPath_t, OutputComponent*> components;

    public:
        Output();

        void addComponent(outputPath_t, OutputComponent*);
        std::map<outputPath_t, OutputComponent*>* getComponents();

        void writeOutput(outputPath_t, outputData_t&);
};
