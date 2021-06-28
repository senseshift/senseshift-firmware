#pragma once

#include <stdio.h>
#include <map>
#include <vector>
#include <utility>

#include "component.h"

typedef char* outputPath_t;

typedef uint8_t intensity_t;

struct Point2D {
    float x, y;
};
typedef Point2D outputPoint_t;

class OutputComponent : public Component
{
    public:

};

typedef std::map<outputPath_t, OutputComponent*> outputComponentsMap_t;

class Output
{
    private:
        outputComponentsMap_t components;

    public:
        Output();

        void addComponent(outputPath_t, OutputComponent*);
        outputComponentsMap_t* getComponents();
};
