#include <Arduino.h>
#include <math.h>

#include "output.h"

outputIntensity_t __calcIntensity(std::list<OutputState> &states)
{
    outputIntensity_t intensity = 0;

    for (auto &_s : states) {
        intensity += _s.intensity;
    }

    return intensity;
}

std::multimap<float, outputPoint_t*> __findClosestPoints(std::list<outputPoint_t*> &pts, Point2D &target)
{
    std::multimap<float, outputPoint_t*> mp = {};

    for (auto &_p : pts)
    {
        if (target == *_p) {
            return { {1, _p} }; // if coord == target, no other needed
        }

        float dx = abs(((float) target.x / UINT16_MAX) - ((float) _p->x / UINT16_MAX)),
            dy = abs(((float) target.y / UINT16_MAX) - ((float) _p->y / UINT16_MAX));

        auto dist = (float) sqrt(pow(dx, 2) + pow(dy, 2));

        mp.insert({dist, _p});
    }

    const size_t num = 4;
    auto end = std::next(mp.begin(), std::min(num, mp.size()));

    return std::multimap<float, outputPoint_t*>(mp.begin(), end);
}

void OutputComponent::setOutputs(std::map<outputPoint_t*, OutputWriter*> &outputs)
{
    this->writers.clear();
    this->writers = writers;

    this->points.clear();
    for(auto &_p : outputs) {
        this->points.push_back(_p.first);
    }

    this->states.clear();
    for(auto &_p : outputs) {
        this->states[_p.first] = {};
    }
}

void OutputComponent::loop()
{
    auto now = millis();

    for (auto _ps = this->states.begin(); _ps != this->states.end();)
    {
        auto &_states = (*_ps).second;

        bool changed = false;

        // if (_states.size() == 0) {
        //     this->states.erase(_ps);
        //     changed = true;
        //     continue;
        // }

        for (auto _s = _states.begin(); _s != _states.end();)
        {
            if ((*_s).endMillis <= now) {
                _states.erase(_s);
                changed = true;
                continue;
            }

            ++_s;
        }

        if (changed) {
            this->writers.at((*_ps).first)->writeOutput(__calcIntensity(_states));
        }

        ++_ps;
    }
}

void OutputComponent::writeOutput(outputData_t& data)
{
    auto closestPoints = __findClosestPoints(this->points, data.point);
    auto now = millis();

    for (auto &_p : closestPoints)
    {
        OutputState state;

        state.endMillis = now + data.duration;
        state.intensity = data.intensity * _p.first;

        this->states[_p.second].push_back(state);

        this->writers.at(_p.second)->writeOutput(__calcIntensity(this->states[_p.second]));
    }
}


Output::Output() {}

void Output::addComponent(outputPath_t path, OutputComponent* c)
{
    this->components[path] = c;
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

    OutputComponent* comp = (*componentSearch).second;

    comp->writeOutput(data);
}