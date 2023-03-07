// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include "openhaptics.h"

#include <bh_utils.hpp>
#include <connection_bhble.hpp>
#include <output_writers/pca9685.hpp>

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include <battery/adc_naive.hpp>
#endif

using namespace OH;
using namespace BH;

static const oh_output_point_t* bhLayout[] = BH_LAYOUT_TACTSUITX16;
static const size_t bhLayoutSize = BH_LAYOUT_TACTSUITX16_SIZE;

// Ouput indices, responsible for x40 => x16 grouping
static const size_t layoutGroupsSize = BH_LAYOUT_TACTSUITX16_GROUPS_SIZE;
static const uint8_t layoutGroups[layoutGroupsSize] = BH_LAYOUT_TACTSUITX16_GROUPS;

void setupMode(OpenHaptics* app) {
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

  app->getOutput()->addComponent(chestFront);
  app->getOutput()->addComponent(chestBack);

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  AbstractBattery* battery = new ADCNaiveBattery(33, { .sampleRate = BATTERY_SAMPLE_RATE }, app, tskNO_AFFINITY);
  app->setBattery(battery);
#endif

  uint8_t serialNumber[BH_SERIAL_NUMBER_LENGTH] = BH_SERIAL_NUMBER;
  ConnectionBHBLE_Config config = {
      .deviceName = BLUETOOTH_NAME,
      .appearance = BH_BLE_APPEARANCE,
      .serialNumber = serialNumber,
  };
  AbstractConnection* bhBleConnection = new ConnectionBHBLE(&config, [app](std::string& value)->void {
    vestX16OutputTransformer(app->getOutput(), value, bhLayout, bhLayoutSize, layoutGroups, layoutGroupsSize);
  }, app);
  app->setConnection(bhBleConnection);
}
