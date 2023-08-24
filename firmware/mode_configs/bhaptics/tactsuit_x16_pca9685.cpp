// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include "senseshift.h"

#include <senseshift/arduino/input/sensor/analog.hpp>
#include <senseshift/arduino/output/pca9685.hpp>
#include <senseshift/battery/sensor.hpp>
#include <senseshift/bh/ble/connection.hpp>
#include <senseshift/bh/devices.hpp>
#include <senseshift/bh/encoding.hpp>
#include <senseshift/freertos/battery.hpp>

using namespace SenseShift;
using namespace SenseShift::Arduino::Output;
using namespace SenseShift::Arduino::Input;
using namespace SenseShift::FreeRTOS::Battery;
using namespace SenseShift::FreeRTOS::Input;
using namespace SenseShift::Battery;
using namespace SenseShift::BH;
using namespace SenseShift::Body::Haptics;

extern SenseShift::SenseShift App;
SenseShift::SenseShift* app = &App;

static const size_t bhLayoutSize = BH_LAYOUT_TACTSUITX16_SIZE;
static const OutputLayout_t bhLayout[BH_LAYOUT_TACTSUITX16_SIZE] = BH_LAYOUT_TACTSUITX16;

// Ouput indices, responsible for x40 => x16 grouping
static const size_t layoutGroupsSize = BH_LAYOUT_TACTSUITX16_GROUPS_SIZE;
static const uint8_t layoutGroups[layoutGroupsSize] = BH_LAYOUT_TACTSUITX16_GROUPS;

void setupMode()
{
    // Configure the PCA9685
    auto pwm = new Adafruit_PWMServoDriver(0x40);
    pwm->begin();
    pwm->setPWMFreq(PWM_FREQUENCY);

    // Assign the pins on the configured PCA9685 to positions on the vest
    auto frontOutputs = PlaneMapper_Margin::mapMatrixCoordinates<VibroPlane::Actuator_t>({
      // clang-format off
      { new PCA9685OutputWriter(pwm, 0), new PCA9685OutputWriter(pwm, 1), new PCA9685OutputWriter(pwm, 2), new PCA9685OutputWriter(pwm, 3) },
      { new PCA9685OutputWriter(pwm, 4), new PCA9685OutputWriter(pwm, 5), new PCA9685OutputWriter(pwm, 6), new PCA9685OutputWriter(pwm, 7) },
      // clang-format on
    });
    auto backOutputs = PlaneMapper_Margin::mapMatrixCoordinates<VibroPlane::Actuator_t>({
      // clang-format off
      { new PCA9685OutputWriter(pwm, 8),  new PCA9685OutputWriter(pwm, 9),  new PCA9685OutputWriter(pwm, 10), new PCA9685OutputWriter(pwm, 11) },
      { new PCA9685OutputWriter(pwm, 12), new PCA9685OutputWriter(pwm, 13), new PCA9685OutputWriter(pwm, 14), new PCA9685OutputWriter(pwm, 15) },
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
