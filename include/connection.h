#pragma once

#include "component.h"

class Connection : public Component
{
    public:
        virtual void setup(void) {};
        virtual void loop(void) {};
};
