#pragma once

#include <map>
#include <list>
#include <vector>

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
        virtual void setup() {};
        virtual void writeOutput(outputIntensity_t intensity) = 0;
};

typedef std::map<outputPoint_t, OutputWriter*> outputMap_t;

class OutputComponent : public Component
{
    protected:
        std::list<outputPoint_t> points{};
        outputMap_t writers{};
        std::map<outputPoint_t, outputState_t> states{};

        void setOutputs(outputMap_t &);

    public:
        OutputComponent(outputMap_t &outputs) { this->setOutputs(outputs); };
        std::list<outputPoint_t>* getOutputPoints(void) { return &this->points; };
        std::map<outputPoint_t, outputState_t>* getOutputStates(void) { return &this->states; };
        virtual void writeOutput(outputData_t&) = 0;
        void setup() override;
};