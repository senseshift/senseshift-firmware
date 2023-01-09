#include <Arduino.h>
#include <Wire.h>

#include "auto_output.h"
#include "openhaptics.h"
#include <utility.hpp>

#include "connections/bhaptics.h"
#include "output_components/closest.h"

class TestOutput : public OutputWriter {
 private:
  uint8_t channel;

 public:
  TestOutput(uint8_t channel) : channel(channel){};
  uint8_t getChannel() { return channel; };
  void writeOutput(outputIntensity_t intensity) {
    Serial.printf("\t> Channel %2u: %3u\n", channel, intensity);
  };
};

void setupMode() {
  auto testOutputs = transformAutoOutput({
      {new TestOutput(0), new TestOutput(1)},
      {new TestOutput(2), new TestOutput(3)},
      {new TestOutput(4), new TestOutput(5)},
      {new TestOutput(6), new TestOutput(7)},
  });

  for (const auto& kv : testOutputs) {
    Serial.printf("> Channel %2u: (%5u, %5u)\n",
                  ((TestOutput*)kv.second)->getChannel(), kv.first.x,
                  kv.first.y);
  }

  auto test = new ClosestOutputComponent(testOutputs);

  App.getOutput()->addComponent(OUTPUT_PATH_ACCESSORY, test);

  for (auto i = 0; i < testOutputs.size(); i++) {
    outputData_t outData{
        outputPoint_t(0, 0),
        (uint8_t)map(i, 0, testOutputs.size() - 1, 0, 255),
    };
    App.getOutput()->writeOutput(OUTPUT_PATH_ACCESSORY, outData);
  }
}
