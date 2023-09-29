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
#include <senseshift/utility.hpp>

using namespace SenseShift;
using namespace SenseShift::Arduino::Output;
using namespace SenseShift::Arduino::Input;
using namespace SenseShift::FreeRTOS::Input;
using namespace SenseShift::Battery;
using namespace SenseShift::BH;
using namespace SenseShift::Body::Haptics;

extern SenseShift::SenseShift App;
SenseShift::SenseShift* app = &App;

static constexpr Body::Hands::HandSide handSide = Body::Hands::HandSide::SENSESHIFT_HAND_SIDE;
static constexpr size_t bhLayoutSize = BH_LAYOUT_TACTGLOVE_SIZE;
// clang-format off
static const OutputLayout (&bhLayout)[bhLayoutSize] = handSide == Body::Hands::HandSide::Left ? BH::TactGloveLeftLayout : BH::TactGloveRightLayout;
// clang-format on

void setupMode()
{
    // Configure PWM pins to their positions on the glove
    // Replace `new PWMOutputWriter(...)` with `nullptr` to disable a specific actuator
    addTactGloveActuators(
      app->getHapticBody(),
      handSide,
      new ActuatorPWM(32), // Thumb
      new ActuatorPWM(33), // Index
      new ActuatorPWM(25), // Middle
      new ActuatorPWM(26), // Ring
      new ActuatorPWM(27), // Little
      new ActuatorPWM(14)  // Wrist
    );

    app->getHapticBody()->setup();

    auto* bhBleConnection = new BLE::Connection(
      {
        .deviceName = BLUETOOTH_NAME,
        .appearance = BH_BLE_APPEARANCE,
        .serialNumber = BH_SERIAL_NUMBER,
      },
      [](std::string& value) -> void {
          Decoder::applyPlain(app->getHapticBody(), value, bhLayout, Effect::Vibro);
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
