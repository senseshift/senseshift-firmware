// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include <utility.hpp>

#include "openhaptics.h"

#include <connection_bhble.hpp>
#include "output_components/closest.h"
#include "output_writers/ledc.h"
#include "output_writers/pca9685.h"

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include "battery/adc_battery.h"
#endif

using namespace OH;
using namespace BH;

#pragma region bHaptics_trash

oh_output_point_t* indexesToPoints[BH_LAYOUT_TACTSUITX40_SIZE] = BH_LAYOUT_TACTSUITX40;

void vestMotorTransformer(std::string& value) {
  for (size_t i = 0; i < 20; i++) {
    uint8_t byte = value[i];
    uint actIndex = i * 2;

    oh_output_data_t output_0{
      .point = *indexesToPoints[actIndex],
      .intensity = static_cast<oh_output_intensity_t>(map(((byte >> 4) & 0xf), 0, 15, 0, OH_OUTPUT_INTENSITY_MAX)),
    };

    App.getOutput()->writeOutput((actIndex < 10 || actIndex >= 30)
                                     ? OUTPUT_PATH_CHEST_FRONT
                                     : OUTPUT_PATH_CHEST_BACK,
                                 output_0);

    oh_output_data_t output_1{
      .point = *indexesToPoints[actIndex + 1],
      .intensity = static_cast<oh_output_intensity_t>(map((byte & 0xf), 0, 15, 0, OH_OUTPUT_INTENSITY_MAX)),
    };

    App.getOutput()->writeOutput((actIndex < 10 || actIndex >= 30)
                                     ? OUTPUT_PATH_CHEST_FRONT
                                     : OUTPUT_PATH_CHEST_BACK,
                                 output_1);
  }
}

#pragma endregion bHaptics_trash

void setupMode() {
  // Configure the PCA9685s
  Adafruit_PWMServoDriver* pwm1 = new Adafruit_PWMServoDriver(0x40);
  pwm1->begin();
  pwm1->setPWMFreq(PWM_FREQUENCY);

  Adafruit_PWMServoDriver* pwm2 = new Adafruit_PWMServoDriver(0x41);
  pwm2->begin();
  pwm2->setPWMFreq(PWM_FREQUENCY);

  // Assign the pins on the configured PCA9685s and PWM pins to locations on the
  // vest
  auto frontOutputs = mapMatrixCoordinates<AbstractOutputWriter>({
      // clang-format off
      {new PCA9685OutputWriter(pwm1, 0),  new PCA9685OutputWriter(pwm1, 1),  new PCA9685OutputWriter(pwm1, 2),  new PCA9685OutputWriter(pwm1, 3)},
      {new PCA9685OutputWriter(pwm1, 4),  new PCA9685OutputWriter(pwm1, 5),  new PCA9685OutputWriter(pwm1, 6),  new PCA9685OutputWriter(pwm1, 7)},
      {new PCA9685OutputWriter(pwm1, 8),  new PCA9685OutputWriter(pwm1, 9),  new PCA9685OutputWriter(pwm1, 10), new PCA9685OutputWriter(pwm1, 11)},
      {new PCA9685OutputWriter(pwm1, 12), new PCA9685OutputWriter(pwm1, 13), new PCA9685OutputWriter(pwm1, 14), new PCA9685OutputWriter(pwm1, 15)},
      {new LEDCOutputWriter(32),          new LEDCOutputWriter(33),          new LEDCOutputWriter(25),          new LEDCOutputWriter(26)},
      // clang-format on
  });
  auto backOutputs = mapMatrixCoordinates<AbstractOutputWriter>({
      // clang-format off
      {new PCA9685OutputWriter(pwm2, 0),  new PCA9685OutputWriter(pwm2, 1),  new PCA9685OutputWriter(pwm2, 2),  new PCA9685OutputWriter(pwm2, 3)},
      {new PCA9685OutputWriter(pwm2, 4),  new PCA9685OutputWriter(pwm2, 5),  new PCA9685OutputWriter(pwm2, 6),  new PCA9685OutputWriter(pwm2, 7)},
      {new PCA9685OutputWriter(pwm2, 8),  new PCA9685OutputWriter(pwm2, 9),  new PCA9685OutputWriter(pwm2, 10), new PCA9685OutputWriter(pwm2, 11)},
      {new PCA9685OutputWriter(pwm2, 12), new PCA9685OutputWriter(pwm2, 13), new PCA9685OutputWriter(pwm2, 14), new PCA9685OutputWriter(pwm2, 15)},
      {new LEDCOutputWriter(27),          new LEDCOutputWriter(14),          new LEDCOutputWriter(12),          new LEDCOutputWriter(13)},
      // clang-format on
  });

  OutputComponent* chestFront = new ClosestOutputComponent(OUTPUT_PATH_CHEST_FRONT, frontOutputs);
  OutputComponent* chestBack = new ClosestOutputComponent(OUTPUT_PATH_CHEST_BACK, backOutputs);

  App.getOutput()->addComponent(chestFront);
  App.getOutput()->addComponent(chestBack);

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
  AbstractConnection* bhBleConnection = new ConnectionBHBLE(&config, vestMotorTransformer, &App);
  App.setConnection(bhBleConnection);
}
