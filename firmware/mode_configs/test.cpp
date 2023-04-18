// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#ifdef UNIT_TEST
  #include "ArduinoFake.h"
#else
  #include "Arduino.h"
#endif

#include "senseshift.h"

#include <haptic_body.hpp>

using namespace OH;

extern SenseShift App;
SenseShift* app = &App;

class TestOutput : public OH::AbstractActuator {
 private:
  uint8_t channel;

 public:
  TestOutput(uint8_t channel) : channel(channel){};
  uint8_t getChannel() { return channel; };
  void writeOutput(oh_output_intensity_t intensity) {};
};

void setupMode() {
  auto testOutputs = mapMatrixCoordinates<AbstractActuator>({
      {new TestOutput(0), new TestOutput(1)},
      {new TestOutput(2), new TestOutput(3)},
      {new TestOutput(4), new TestOutput(5)},
      {new TestOutput(6), new TestOutput(7)},
  });

  auto test = new HapticPlane_Closest(testOutputs);
  App.getHapticBody()->addComponent(OUTPUT_PATH_ACCESSORY, test);
}

void loopMode() {
}
