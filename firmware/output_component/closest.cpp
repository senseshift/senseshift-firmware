#include <algorithm>

#include "output_components/closest.h"

std::multimap<float, outputPoint_t> __findClosestPoints(std::list<outputPoint_t> &pts, Point2D &target)
{
    std::multimap<float, outputPoint_t> mp = {};

    for (auto &_p : pts)
    {
        if (target == _p) {
            return { {1, _p} }; // if coord == target, no other needed
        }

        float dx = abs(((float) target.x / UINT16_MAX) - ((float) _p.x / UINT16_MAX)),
            dy = abs(((float) target.y / UINT16_MAX) - ((float) _p.y / UINT16_MAX));

        auto dist = (float) sqrt(pow(dx, 2) + pow(dy, 2));

        mp.insert({dist, _p});
    }

    auto nearest = std::min_element(
        mp.begin(), mp.end(),
        [](const std::pair<float, outputPoint_t> &a, const std::pair<float, outputPoint_t> &b) {
            return a.first < b.first;
        }
    );

    return {
        {1, nearest->second},
    };
}

void ClosestOutputComponent::setOutputs(std::map<outputPoint_t, OutputWriter*> &outputs)
{
    this->writers.clear();
    this->writers = outputs;

    this->points.clear();
    for(auto &_p : outputs) {
        this->points.push_back(_p.first);
    }

    this->states.clear();
    for(auto &_p : outputs) {
        this->states[_p.first] = {};
    }
}

void ClosestOutputComponent::writeOutput(outputData_t& data)
{
    auto closestPoints = __findClosestPoints(this->points, data.point);

    for (auto &_p : closestPoints)
    {
        auto state = &this->states[_p.second];

        state->intensity = data.intensity * _p.first;

        this->writers.at(_p.second)->writeOutput(state->intensity);
    }
}