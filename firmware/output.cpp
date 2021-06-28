#include "output.h"

Output::Output() {}

void Output::addComponent(outputPath_t path, OutputComponent* c)
{
    this->components[path] = c;
}

outputComponentsMap_t* Output::getComponents()
{
    return &this->components;
}