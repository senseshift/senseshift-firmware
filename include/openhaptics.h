#pragma once

#include <vector>

#include "component.h"
#include "output.h"

class OpenHaptics final
{
    private:
        std::vector<Component*> components {};
        Output* output;

    public:
        OpenHaptics();
        std::vector<Component*> getComponents() { return this->components; };
        void registerComponent(Component*);
        
        Output* getOutput() { return this->output; };
        void addOutputComponent(outputPath_t, OutputComponent*);

        void setup(void);
        void loop(void);
};

extern OpenHaptics App;
