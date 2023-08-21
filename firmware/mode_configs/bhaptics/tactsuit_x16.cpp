// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include "senseshift.h"

#include <output_writers/pwm.hpp>
#include <senseshift/bh/ble/connection.hpp>
#include <senseshift/bh/devices.hpp>
#include <senseshift/bh/encoding.hpp>

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include <battery/adc_naive.hpp>
#endif

using namespace OH;
using namespace SenseShift;
using namespace SenseShift::Body::Haptics;

extern SenseShift::SenseShift App;
SenseShift::SenseShift* app = &App;

static const size_t bhLayoutSize = BH_LAYOUT_TACTSUITX16_SIZE;
static const BH::OutputLayout_t bhLayout[BH_LAYOUT_TACTSUITX16_SIZE] = BH_LAYOUT_TACTSUITX16;

// Ouput indices, responsible for x40 => x16 grouping
static const size_t layoutGroupsSize = BH_LAYOUT_TACTSUITX16_GROUPS_SIZE;
static const uint8_t layoutGroups[layoutGroupsSize] = BH_LAYOUT_TACTSUITX16_GROUPS;

void setupMode()
{
    // Configure PWM pins to their positions on the vest
    auto frontOutputs = PlaneMapper_Margin::mapMatrixCoordinates<AbstractActuator>({
      // clang-format off
      { new PWMOutputWriter(32), new PWMOutputWriter(33), new PWMOutputWriter(25), new PWMOutputWriter(26) },
      { new PWMOutputWriter(27), new PWMOutputWriter(14), new PWMOutputWriter(12), new PWMOutputWriter(13) },
      // clang-format on
    });
    auto backOutputs = PlaneMapper_Margin::mapMatrixCoordinates<AbstractActuator>({
      // clang-format off
      { new PWMOutputWriter(19), new PWMOutputWriter(18), new PWMOutputWriter(5), new PWMOutputWriter(17) },
      { new PWMOutputWriter(16), new PWMOutputWriter(4), new PWMOutputWriter(2), new PWMOutputWriter(15) },
      // clang-format on
    });

    app->getHapticBody()->addTarget(Target::ChestFront, new VibroPlane_Closest(frontOutputs));
    app->getHapticBody()->addTarget(Target::ChestBack, new VibroPlane_Closest(backOutputs));

    app->getHapticBody()->setup();

    auto* bhBleConnection = new BH::BLE::Connection(
      {
        .deviceName = BLUETOOTH_NAME,
        .appearance = BH_BLE_APPEARANCE,
        .serialNumber = BH_SERIAL_NUMBER,
      },
      [](std::string& value) -> void {
          BH::Decoder::applyVestGrouped(app->getHapticBody(), value, bhLayout, layoutGroups);
      },
      app
    );
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

void loopMode()
{
    // Free up the Arduino loop task
    vTaskDelete(NULL);
}
