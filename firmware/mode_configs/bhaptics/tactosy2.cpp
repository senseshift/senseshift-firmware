// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include "senseshift.h"

#include <senseshift/arduino/input/sensor/analog.hpp>
#include <senseshift/arduino/output/ledc.hpp>
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

extern Application App;
Application* app = &App;

static const std::array<Position, BH_LAYOUT_TACTOSY2_SIZE> bhLayout = { BH_LAYOUT_TACTOSY2 };

void setupMode()
{
    // Configure PWM pins to their positions on the forearm
    auto forearmOutputs = PlaneMapper_Margin::mapMatrixCoordinates<FloatPlane::Actuator>({
      // clang-format off
      { new LedcOutput(32), new LedcOutput(33), new LedcOutput(25) },
      { new LedcOutput(26), new LedcOutput(27), new LedcOutput(14) },
      // clang-format on
    });

    app->getVibroBody()->addTarget(Target::Accessory, new FloatPlane_Closest(forearmOutputs));

    app->getVibroBody()->setup();

    auto* bhBleConnection = new BLE::Connection(
      {
        .deviceName = BLUETOOTH_NAME,
        .appearance = BH_BLE_APPEARANCE,
        .serialNumber = BH_SERIAL_NUMBER,
      },
      [](std::string& value) -> void {
          Decoder::applyPlain(app->getVibroBody(), value, bhLayout, Effect::Vibro, Target::Accessory);
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
