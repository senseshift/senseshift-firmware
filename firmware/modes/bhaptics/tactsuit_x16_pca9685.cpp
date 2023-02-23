// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include <utility.hpp>

#include "openhaptics.h"

#include <connection_bhble.hpp>
#include "output_components/closest.h"
#include "output_writers/pca9685.h"

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include "battery/adc_battery.h"
#endif

using namespace OH;
using namespace BH;

#define PWM_FREQUENCY 60

#pragma region bHaptics_trash

oh_output_point_t* indexesToPoints[BH_LAYOUT_TACTSUITX16_SIZE] = BH_LAYOUT_TACTSUITX16;

// Ouput indices, responsible for x40 => x16 grouping
uint8_t groups[BH_LAYOUT_TACTSUITX16_GROUPS_SIZE] = BH_LAYOUT_TACTSUITX16_GROUPS;

void vestMotorTransformer(std::string& value) {
  uint8_t result[BH_LAYOUT_TACTSUITX16_SIZE];

  // Unpack values
  for (auto i = 0; i < 20; i++) {
    uint8_t byte = value[i];
    uint actIndex = i * 2;

    result[actIndex] = (byte >> 4) & 0xf;
    result[actIndex + 1] = (byte & 0xf);
  }

  // Assign max value into each group
  for (auto i = 0; i < 16; i++) {
    auto groupIndex = groups[i];

    if (groupIndex % 10 >= 4) {
      // Top 3 rows of x40
      auto maxValue = max(result[groupIndex], max(result[groupIndex + 2], result[groupIndex + 4]));

      result[groupIndex] = maxValue;
      result[groupIndex + 2] = maxValue;
      result[groupIndex + 4] = maxValue;
    } else {
      // Bottom 2 rows of x40
      auto maxValue = max(result[groupIndex], result[groupIndex + 2]);

      result[groupIndex] = maxValue;
      result[groupIndex + 2] = maxValue;
    }
  }

  for (uint8_t i = 0; i < BH_LAYOUT_TACTSUITX16_SIZE; i++) {
    // take only meaningful values
    if (!contains(groups, i)) {
      continue;
    }

    oh_output_data_t output{
      .point = *indexesToPoints[i],
      .intensity = static_cast<oh_output_intensity_t>(map(result[i], 0, 15, 0, OH_OUTPUT_INTENSITY_MAX)),
    };

    App.getOutput()->writeOutput(
      (i < 10 || i >= 30) ? OUTPUT_PATH_CHEST_FRONT : OUTPUT_PATH_CHEST_BACK,
      output
    );
  }
}

#pragma endregion bHaptics_trash

void setupMode() {
  // Configure the PCA9685
  auto pwm = new Adafruit_PWMServoDriver(0x40);
  pwm->begin();
  pwm->setPWMFreq(PWM_FREQUENCY);

  // Assign the pins on the configured PCA9685 to positions on the vest
  auto frontOutputs = mapMatrixCoordinates<AbstractOutputWriter>({
      // clang-format off
      {new PCA9685OutputWriter(pwm, 0), new PCA9685OutputWriter(pwm, 1), new PCA9685OutputWriter(pwm, 2), new PCA9685OutputWriter(pwm, 3)},
      {new PCA9685OutputWriter(pwm, 4), new PCA9685OutputWriter(pwm, 5), new PCA9685OutputWriter(pwm, 6), new PCA9685OutputWriter(pwm, 7)},
      // clang-format on
  });
  auto backOutputs = mapMatrixCoordinates<AbstractOutputWriter>({
      // clang-format off
      {new PCA9685OutputWriter(pwm, 8),  new PCA9685OutputWriter(pwm, 9),  new PCA9685OutputWriter(pwm, 10), new PCA9685OutputWriter(pwm, 11)},
      {new PCA9685OutputWriter(pwm, 12), new PCA9685OutputWriter(pwm, 13), new PCA9685OutputWriter(pwm, 14), new PCA9685OutputWriter(pwm, 15)},
      // clang-format on
  });

  OutputComponent* chestFront = new ClosestOutputComponent(OUTPUT_PATH_CHEST_FRONT, frontOutputs);
  OutputComponent* chestBack = new ClosestOutputComponent(OUTPUT_PATH_CHEST_BACK, backOutputs);

  App.addOutputComponent(chestFront);
  App.addOutputComponent(chestBack);

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  AbstractBattery* battery = new ADCBattery(33, { .sampleRate = BATTERY_SAMPLE_RATE }, &App);
  App.setBattery(battery);
#endif

  uint8_t serialNumber[BH_SERIAL_NUMBER_LENGTH] = BH_SERIAL_NUMBER;
  ConnectionBHBLE_Config config = {
      .deviceName = BLUETOOTH_NAME,
      .appearance = BH_BLE_APPEARANCE,
      .serialNumber = serialNumber,
  };
  AbstractConnection* bhBleConnection = new ConnectionBHBLE(&config, vestMotorTransformer, &App, tskNO_AFFINITY);
  App.setConnection(bhBleConnection);
}
