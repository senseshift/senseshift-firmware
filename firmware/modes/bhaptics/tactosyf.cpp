// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include "openhaptics.h"

#include <connection_bhble.hpp>
#include <output_writers/pwm.hpp>

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include <battery/adc_naive.hpp>
#endif

using namespace OH;
using namespace BH;

#pragma region bHaptics_trash

oh_output_point_t* indexesToPoints[BH_LAYOUT_TACTOSYF_SIZE] = BH_LAYOUT_TACTOSYF;

void vestMotorTransformer(std::string& value) {
  for (size_t i = 0; i < BH_LAYOUT_TACTOSYF_SIZE; i++) {
  uint8_t byte = value[i];

    oh_output_data_t output_0{
      .point = *indexesToPoints[i],
      .intensity = static_cast<oh_output_intensity_t>(map(byte, 0, 100, 0, OH_OUTPUT_INTENSITY_MAX)),
    };

    App.getOutput()->writeOutput(OUTPUT_PATH_ACCESSORY, output_0);
  }
}

#pragma endregion bHaptics_trash

void setupMode() {
  // Configure PWM pins to their positions on the feet
  auto footOutputs = mapMatrixCoordinates<AbstractOutputWriter>({
      // clang-format off
      {new PWMOutputWriter(32), new PWMOutputWriter(33), new PWMOutputWriter(25)}
      // clang-format on
  });

  OutputComponent* foot = new ClosestOutputComponent(OUTPUT_PATH_ACCESSORY, footOutputs);
  App.getOutput()->addComponent(foot);

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  AbstractBattery* battery = new ADCNaiveBattery(33, { .sampleRate = BATTERY_SAMPLE_RATE }, &App, tskNO_AFFINITY);
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
