// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include <utility.hpp>

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

static const Body::Hands::HandSide_t handSide = Body::Hands::HandSide::SENSESHIFT_HAND_SIDE;
static const size_t bhLayoutSize = BH_LAYOUT_TACTGLOVE_SIZE;
// clang-format off
static const BH::OutputLayout_t (&bhLayout)[bhLayoutSize] = handSide == Body::Hands::HandSide::Left ? BH::TactGloveLeftLayout : BH::TactGloveRightLayout;
// clang-format on

void setupMode()
{
    // Configure PWM pins to their positions on the glove
    // Replace `new PWMOutputWriter(...)` with `nullptr` to disable a specific actuator
    BH::addTactGloveActuators(
      app->getHapticBody(),
      handSide,
      new PWMOutputWriter(32), // Thumb
      new PWMOutputWriter(33), // Index
      new PWMOutputWriter(25), // Middle
      new PWMOutputWriter(26), // Ring
      new PWMOutputWriter(27), // Little
      new PWMOutputWriter(14)  // Wrist
    );

    app->getHapticBody()->setup();

    auto* bhBleConnection = new BH::BLE::Connection(
      {
        .deviceName = BLUETOOTH_NAME,
        .appearance = BH_BLE_APPEARANCE,
        .serialNumber = BH_SERIAL_NUMBER,
      },
      [](std::string& value) -> void {
          BH::Decoder::applyPlain(app->getHapticBody(), value, bhLayout, Effect_t::Vibro);
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
