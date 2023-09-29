// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include "senseshift.h"

#include <senseshift/arduino/input/sensor/analog.hpp>
#include <senseshift/arduino/output/actuator/pwm.hpp>
#include <senseshift/battery/sensor.hpp>
#include <senseshift/bh/ble/connection.hpp>
#include <senseshift/bh/devices.hpp>
#include <senseshift/bh/encoding.hpp>
#include <senseshift/freertos/input/sensor.hpp>

using namespace SenseShift;
using namespace SenseShift::Arduino::Output;
using namespace SenseShift::Arduino::Input;
using namespace SenseShift::FreeRTOS::Input;
using namespace SenseShift::Battery;
using namespace SenseShift::BH;
using namespace SenseShift::Body::Haptics;

extern SenseShift::SenseShift App;
SenseShift::SenseShift* app = &App;

static constexpr size_t bhLayoutSize = BH_LAYOUT_TACTSUITX16_SIZE;
static const OutputLayout bhLayout[BH_LAYOUT_TACTSUITX16_SIZE] = BH_LAYOUT_TACTSUITX16;

// Ouput indices, responsible for x40 => x16 grouping
static constexpr size_t layoutGroupsSize = BH_LAYOUT_TACTSUITX16_GROUPS_SIZE;
static const uint8_t layoutGroups[layoutGroupsSize] = BH_LAYOUT_TACTSUITX16_GROUPS;

void setupMode()
{
    // Configure PWM pins to their positions on the vest
    auto frontOutputs = PlaneMapper_Margin::mapMatrixCoordinates<VibroPlane::Actuator>({
      // clang-format off
      { new ActuatorPWM(32), new ActuatorPWM(33), new ActuatorPWM(25), new ActuatorPWM(26) },
      { new ActuatorPWM(27), new ActuatorPWM(14), new ActuatorPWM(12), new ActuatorPWM(13) },
      // clang-format on
    });
    auto backOutputs = PlaneMapper_Margin::mapMatrixCoordinates<VibroPlane::Actuator>({
      // clang-format off
      { new ActuatorPWM(19), new ActuatorPWM(18), new ActuatorPWM(5), new ActuatorPWM(17) },
      { new ActuatorPWM(16), new ActuatorPWM(4), new ActuatorPWM(2), new ActuatorPWM(15)  },
      // clang-format on
    });

    app->getHapticBody()->addTarget(Target::ChestFront, new VibroPlane_Closest(frontOutputs));
    app->getHapticBody()->addTarget(Target::ChestBack, new VibroPlane_Closest(backOutputs));

    app->getHapticBody()->setup();

    auto* bhBleConnection = new BLE::Connection(
      {
        .deviceName = BLUETOOTH_NAME,
        .appearance = BH_BLE_APPEARANCE,
        .serialNumber = BH_SERIAL_NUMBER,
      },
      [](std::string& value) -> void {
          Decoder::applyVestGrouped(app->getHapticBody(), value, bhLayout, layoutGroups);
      },
      app
    );
    bhBleConnection->begin();

#if defined(SENSESHIFT_BATTERY_ENABLED) && SENSESHIFT_BATTERY_ENABLED == true
    auto* battery = new TaskedSensor<BatteryState>(
      new BatterySensor(new NaiveBatterySensor(new AnalogSensor(36)), app),
      SENSESHIFT_BATTERY_SAMPLE_RATE,
      { "ADC Battery", 4096, SENSESHIFT_BATTERY_TASK_PRIORITY, tskNO_AFFINITY }
    );
    battery->begin();
#endif
}

void loopMode()
{
    // Free up the Arduino loop task
    vTaskDelete(NULL);
}
