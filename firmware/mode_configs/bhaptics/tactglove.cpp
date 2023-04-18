// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include <utility.hpp>

#include "senseshift.h"

#include <bh_utils.hpp>
#include <connection_bhble.hpp>
#include <output_writers/pwm.hpp>

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include <battery/adc_naive.hpp>
#endif

using namespace OH;
using namespace BH;

extern SenseShift App;
SenseShift* app = &App;

#pragma region bHaptics_trash

// TODO: all of this will need to be re-written to use the new output paths system, when time comes

static const uint16_t _bh_size_x = 6;
static const uint16_t _bh_size_y = 1;

inline oh_output_point_t* make_point(oh_output_coord_t x, oh_output_coord_t y) {
  return mapPoint(x, y, (oh_output_coord_t) (_bh_size_x - 1), (oh_output_coord_t) (_bh_size_y - 1));
}

static const uint16_t bhLayoutSize = _bh_size_x * _bh_size_y;
static const oh_output_point_t* bhLayout[bhLayoutSize] = {
    // clang-format off

    // Thumb, Index, Middle, Ring, Pinky
    make_point(0, 0), make_point(1, 0), make_point(2, 0), make_point(3, 0), make_point(4, 0),
    // Wrist
    make_point(5, 0)

    // clang-format on
};

#pragma endregion bHaptics_trash

void setupMode() {
  // Configure PWM pins to their positions on the glove
  auto gloveOutputs = mapMatrixCoordinates<AbstractActuator>({
      // clang-format off
      {
        // Thumb, Index, Middle, Ring, Pinky
        new PWMOutputWriter(32), new PWMOutputWriter(33), new PWMOutputWriter(25), new PWMOutputWriter(26), new PWMOutputWriter(27),
        // Wrist
        new PWMOutputWriter(14)
      },
      // clang-format on
  });

  auto* glove = new HapticPlane_Closest(gloveOutputs);
  app->getHapticBody()->addComponent(OUTPUT_PATH_ACCESSORY, glove);

  app->getHapticBody()->setup();

  uint8_t serialNumber[BH_SERIAL_NUMBER_LENGTH] = BH_SERIAL_NUMBER;
  ConnectionBHBLE_Config config = {
      .deviceName = BLUETOOTH_NAME,
      .appearance = BH_BLE_APPEARANCE,
      .serialNumber = serialNumber,
  };
  auto* bhBleConnection = new ConnectionBHBLE(config, [](std::string& value)->void {
    plainOutputTransformer(app->getHapticBody(), value, bhLayout, bhLayoutSize, OUTPUT_PATH_ACCESSORY);
  }, app);
  bhBleConnection->begin();

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  auto* battery = new ADCNaiveBattery(36, { .sampleRate = BATTERY_SAMPLE_RATE }, app, tskNO_AFFINITY);
  battery->begin();
#endif
}

void loopMode() {
  // Free up the Arduino loop task
  vTaskDelete(NULL);
}
