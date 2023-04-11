// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include "openhaptics.h"

#include <bh_utils.hpp>
#include <connection_bhble.hpp>
#include <abstract_output_writer.hpp>
#include <haptic_body.hpp>

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include <battery/adc_naive.hpp>
#endif

using namespace OH;
using namespace BH;

extern OpenHaptics App;
OpenHaptics* app = &App;

class TestOutput : public OH::AbstractActuator {
 private:
  uint8_t channel;

 public:
  TestOutput(uint8_t channel) : channel(channel){};
  uint8_t getChannel() { return channel; };
  void writeOutput(oh_output_intensity_t intensity) {
    Serial.printf("\t> Channel %2u: %3u\n", channel, intensity);
  };
};

void setupMode() {
  auto testOutputs = mapMatrixCoordinates<AbstractActuator>({
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

  auto test = new HapticPlane_Closest(OUTPUT_PATH_ACCESSORY, testOutputs);

  oh_output_writers_map_t layout{
    {oh_output_point_t(0, 0), new TestOutput(8)},
  };
  auto test2 = new HapticPlane_Closest(OUTPUT_PATH_ACCESSORY, layout);

  App.getHapticBody()->addComponent(test);

  for (auto i = 0; i < testOutputs.size(); i++) {
    oh_output_data_t outData{
        oh_output_point_t(0, 0),
        (uint8_t)map(i, 0, testOutputs.size() - 1, 0, 255),
    };
    App.getHapticBody()->writeOutput(OUTPUT_PATH_ACCESSORY, outData);
  }
}

void loopMode() {
  // Free up the Arduino loop task
  vTaskDelete(NULL);
}
