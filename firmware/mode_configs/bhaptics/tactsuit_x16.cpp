// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

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

static const oh_output_point_t* bhLayout[] = BH_LAYOUT_TACTSUITX16;
static const size_t bhLayoutSize = BH_LAYOUT_TACTSUITX16_SIZE;

// Ouput indices, responsible for x40 => x16 grouping
static const size_t layoutGroupsSize = BH_LAYOUT_TACTSUITX16_GROUPS_SIZE;
static const uint8_t layoutGroups[layoutGroupsSize] = BH_LAYOUT_TACTSUITX16_GROUPS;

void setupMode() {
  // Configure PWM pins to their positions on the vest
  auto frontOutputs = PlaneMapper_Margin::mapMatrixCoordinates<AbstractActuator>({
      // clang-format off
      {new PWMOutputWriter(32), new PWMOutputWriter(33), new PWMOutputWriter(25), new PWMOutputWriter(26)},
      {new PWMOutputWriter(27), new PWMOutputWriter(14), new PWMOutputWriter(12), new PWMOutputWriter(13)},
      // clang-format on
  });
  auto backOutputs = PlaneMapper_Margin::mapMatrixCoordinates<AbstractActuator>({
      // clang-format off
      {new PWMOutputWriter(19), new PWMOutputWriter(18), new PWMOutputWriter(5), new PWMOutputWriter(17)},
      {new PWMOutputWriter(16), new PWMOutputWriter(4), new PWMOutputWriter(2), new PWMOutputWriter(15)},
      // clang-format on
  });

  auto* chestFront = new HapticPlane_Closest(frontOutputs);
  auto* chestBack = new HapticPlane_Closest(backOutputs);

  app->getHapticBody()->addComponent(OUTPUT_PATH_CHEST_FRONT, chestFront);
  app->getHapticBody()->addComponent(OUTPUT_PATH_CHEST_BACK, chestBack);

  app->getHapticBody()->setup();

  uint8_t serialNumber[BH_SERIAL_NUMBER_LENGTH] = BH_SERIAL_NUMBER;
  ConnectionBHBLE_Config config = {
      .deviceName = BLUETOOTH_NAME,
      .appearance = BH_BLE_APPEARANCE,
      .serialNumber = serialNumber,
  };
  auto* bhBleConnection = new ConnectionBHBLE(config, [](std::string& value)->void {
    vestX16OutputTransformer(app->getHapticBody(), value, bhLayout, bhLayoutSize, layoutGroups, layoutGroupsSize);
  }, app);
  bhBleConnection->begin();

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  auto* battery = new BatterySensor(
    new ADCNaiveBattery(36),
    &App,
    { .sampleRate = BATTERY_SAMPLE_RATE },
    { "ADC Battery", 4096, BATTERY_TASK_PRIORITY, tskNO_AFFINITY }
  );
  battery->begin();
#endif
}

void loopMode() {
  // Free up the Arduino loop task
  vTaskDelete(NULL);
}
