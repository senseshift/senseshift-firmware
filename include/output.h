#pragma once

#include <stdio.h>
#include <map>
#include <list>
#include <vector>
#include <utility>

#include <string>

#include "config/output.h"

#include "component.h"

typedef uint8_t outputPath_t;

struct Point2D {
    uint16_t x, y;

    Point2D(uint16_t x = 0, uint16_t y = 0) : x(x), y(y) {}

    bool const operator==(const Point2D &o) const {
        return x == o.x && y == o.y;
    }

    bool const operator<(const Point2D &o) const {
        return std::tie(x, y) < std::tie(o.x, o.y);
    }
};
typedef Point2D outputPoint_t;

typedef uint16_t outputIntensity_t;

struct OutputData {
    outputPoint_t point;
    outputIntensity_t intensity;
};
typedef OutputData outputData_t;

struct OutputState {
    outputIntensity_t intensity;
};
typedef OutputState outputState_t;

class OutputWriter
{
    public:
        virtual void writeOutput(outputIntensity_t intensity) = 0;
};

class OutputComponent : public Component
{
    private:
        std::list<outputPoint_t> points{};
        std::map<outputPoint_t, OutputWriter*> writers{};
        std::map<outputPoint_t, outputState_t> states{};

    public:
        void setOutputs(std::map<outputPoint_t, OutputWriter*> &);
        std::list<outputPoint_t>* getOutputPoints(void) { return &this->points; };
        std::map<outputPoint_t, outputState_t>* getOutputStates(void) { return &this->states; };
        void writeOutput(outputData_t&);
};

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
