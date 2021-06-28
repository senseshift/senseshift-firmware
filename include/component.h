#pragma once

class Component
{
    public:
        virtual void setup(void) = 0;
        virtual void loop(void) = 0;
};
