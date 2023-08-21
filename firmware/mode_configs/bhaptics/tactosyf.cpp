// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include "senseshift.h"

#include <senseshift/arduino/input/sensor/analog.hpp>
#include <senseshift/arduino/output/pwm.hpp>
#include <senseshift/battery/sensor.hpp>
#include <senseshift/bh/ble/connection.hpp>
#include <senseshift/bh/devices.hpp>
#include <senseshift/bh/encoding.hpp>

using namespace OH;
using namespace SenseShift;
using namespace SenseShift::Arduino::Output;
using namespace SenseShift::Arduino::Input;
using namespace SenseShift::Battery;
using namespace SenseShift::BH;
using namespace SenseShift::Body::Haptics;

extern SenseShift::SenseShift App;
SenseShift::SenseShift* app = &App;

static const size_t bhLayoutSize = BH_LAYOUT_TACTOSYF_SIZE;
static const Position_t bhLayout[bhLayoutSize] = BH_LAYOUT_TACTOSYF;

void setupMode()
{
    // Configure PWM pins to their positions on the feet
    auto footOutputs = PlaneMapper_Margin::mapMatrixCoordinates<VibroPlane::Actuator_t>({
      // clang-format off
      { new PWMOutputWriter(32) },
      { new PWMOutputWriter(33) },
      { new PWMOutputWriter(25) },
      // clang-format on
    });

    app->getHapticBody()->addTarget(Target::Accessory, new VibroPlane_Closest(footOutputs));

    app->getHapticBody()->setup();

    auto* bhBleConnection = new BLE::Connection(
      {
        .deviceName = BLUETOOTH_NAME,
        .appearance = BH_BLE_APPEARANCE,
        .serialNumber = BH_SERIAL_NUMBER,
      },
      [](std::string& value) -> void {
          Decoder::applyPlain(app->getHapticBody(), value, bhLayout, Effect::Vibro, Target::Accessory);
      },
      app
    );
    bhBleConnection->begin();

#if defined(SENSESHIFT_BATTERY_ENABLED) && SENSESHIFT_BATTERY_ENABLED == true
    auto* battery = new BatterySensor(
      new NaiveBatterySensor(new AnalogSensor(36)),
      &App,
      { .sampleRate = SENSESHIFT_BATTERY_SAMPLE_RATE },
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
