#pragma once

#include <list>
#include <map>
#include <vector>

#include <abstract_component.hpp>
#include <abstract_output_writer.hpp>
#include <vec2.hpp>

typedef uint8_t outputPath_t;

typedef OH::Vec2b outputPoint_t;

struct OutputData {
  outputPoint_t point;
  oh_outputIntensity_t intensity;
};
typedef OutputData outputData_t;

struct OutputState {
  oh_outputIntensity_t intensity;
};
typedef OutputState outputState_t;

typedef std::map<outputPoint_t, OH::AbstractOutputWriter*> outputMap_t;

class OutputComponent : public OH::AbstractComponent {
 protected:
  std::list<outputPoint_t> points{};
  outputMap_t writers{};
  std::map<outputPoint_t, outputState_t> states{};

  void setOutputs(outputMap_t&);

 public:
  OutputComponent(outputMap_t& outputs) { this->setOutputs(outputs); };
  std::list<outputPoint_t>* getOutputPoints(void) { return &this->points; };
  std::map<outputPoint_t, outputState_t>* getOutputStates(void) {
    return &this->states;
  };
  virtual void writeOutput(outputData_t&) = 0;
  void setup() override;
};
