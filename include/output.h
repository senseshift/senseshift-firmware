#pragma once

#include <stdio.h>
#include <map>
#include <vector>
#include <utility>

#include "config/output.h"

#include "component.h"

typedef uint8_t outputPath_t;

struct Point2D {
    uint16_t x, y;

    bool const operator==(const Point2D &o) const {
        return std::tie(x, y) == std::tie(o.x, o.y);
    }

    bool const operator<(const Point2D &o) const {
        return std::tie(x, y) < std::tie(o.x, o.y);
    }
};
typedef Point2D outputPoint_t;

typedef uint16_t outputDuration_t;
typedef uint16_t outputIntensity_t;

struct OutputData {
    outputPoint_t point;
    outputDuration_t duration;
    outputIntensity_t intensity;
};
typedef OutputData outputData_t;

struct OutputState {
    unsigned long long endMillis;
    outputIntensity_t intensity;
};
typedef OutputState outputState_t;

class OutputWriter
{
    void writeOutput(outputIntensity_t intensity);
};

class OutputComponent : public Component
{
    private:
        std::map<outputPoint_t, std::vector<outputState_t>> points;

    public:
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
