#include "openhaptics.h"
#include "output.h"

Output::Output() {}

void Output::addComponent(outputPath_t path, OutputComponent* c)
{
    this->components[path] = c;
    App.registerComponent(c);
}

std::map<outputPath_t, OutputComponent*>* Output::getComponents()
{
    return &this->components;
}

void Output::writeOutput(outputPath_t path, outputData_t& data)
{
    auto componentSearch = this->getComponents()->find(path);

    if (componentSearch == this->getComponents()->end()) {
        // if no requested component exists, skip
        return;
    }

    (*componentSearch).second->writeOutput(data);
}
