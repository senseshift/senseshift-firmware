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
#include "battery/adc_naive.hpp"
#endif

using namespace OH;
using namespace SenseShift;
using namespace SenseShift::Body::Haptics;

extern SenseShift::SenseShift App;
SenseShift::SenseShift* app = &App;

static const size_t bhLayoutSize = BH_LAYOUT_TACTAL_SIZE;
static const Position_t bhLayout[bhLayoutSize] = BH_LAYOUT_TACTAL;

void setupMode()
{
    // Configure PWM pins to their positions on the face
    const auto faceOutputs = PlaneMapper_Margin::mapMatrixCoordinates<AbstractActuator>({
      // clang-format off
      { new PWMOutputWriter(32), new PWMOutputWriter(33), new PWMOutputWriter(25), new PWMOutputWriter(26), new PWMOutputWriter(27), new PWMOutputWriter(14) },
      // clang-format on
    });

    app->getHapticBody()->addTarget(Target::FaceFront, new VibroPlane_Closest(faceOutputs));

    app->getHapticBody()->setup();

    auto* bhBleConnection = new BH::BLE::Connection(
      {
        .deviceName = BLUETOOTH_NAME,
        .appearance = BH_BLE_APPEARANCE,
        .serialNumber = BH_SERIAL_NUMBER,
      },
      [](std::string& value) -> void {
          BH::Decoder::applyPlain(app->getHapticBody(), value, bhLayout, Effect::Vibro, Target::FaceFront);
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
