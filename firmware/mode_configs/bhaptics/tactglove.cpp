// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include <utility.hpp>

#include "openhaptics.h"

#include <connection_bhble.hpp>
#include <output_writers/pwm.hpp>

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include <battery/adc_naive.hpp>
#endif

using namespace OH;
using namespace BH;

extern OpenHaptics App;

#pragma region bHaptics_trash

const uint16_t _bh_size_x = 6;
const uint16_t _bh_size_y = 1;

inline oh_output_point_t* make_point(oh_output_coord_t x, oh_output_coord_t y) {
  return getPoint(x, y, (oh_output_coord_t) (_bh_size_x - 1), (oh_output_coord_t) (_bh_size_y - 1));
}

oh_output_point_t* indexesToPoints[_bh_size_x * _bh_size_y] = {
    make_point(0, 0), make_point(1, 0), make_point(2, 0),
    make_point(3, 0), make_point(4, 0), make_point(5, 0)};

void vestMotorTransformer(std::string& value) {
  for (size_t i = 0; i < _bh_size_x; i++) {
    uint8_t byte = value[i];
    oh_output_data_t output_0;
    output_0.point = *indexesToPoints[i];
    output_0.intensity = map(byte, 0, 100, 0, UINT16_MAX);
    App.getOutput()->writeOutput(OUTPUT_PATH_ACCESSORY, output_0);
  }
}

#pragma endregion bHaptics_trash

void setupMode() {
  // Configure PWM pins to their positions on the glove
  auto gloveOutputs = mapMatrixCoordinates<AbstractOutputWriter>({
      // clang-format off
      {new PWMOutputWriter(32), new PWMOutputWriter(33), new PWMOutputWriter(25), new PWMOutputWriter(26), new PWMOutputWriter(27), new PWMOutputWriter(14)},
      // clang-format on
  });

  OutputComponent* glove = new ClosestOutputComponent(gloveOutputs);
  App.getOutput()->addComponent(OUTPUT_PATH_ACCESSORY, glove);

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  AbstractBattery* battery = new ADCNaiveBattery(33, { .sampleRate = BATTERY_SAMPLE_RATE }, &App);
  App.setBattery(battery);
#endif

  uint8_t serialNumber[BH_SERIAL_NUMBER_LENGTH] = BH_SERIAL_NUMBER;
  ConnectionBHBLE_Config config = {
      .deviceName = BLUETOOTH_NAME,
      .appearance = BH_BLE_APPEARANCE,
      .serialNumber = serialNumber,
  };
  AbstractConnection* bhBleConnection = new ConnectionBHBLE(&config, vestMotorTransformer, &App);
  App.setConnection(bhBleConnection);
}
