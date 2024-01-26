// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include "senseshift.h"
#include <senseshift/arduino/output/pca9685.hpp>
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

static const std::array<OutputLayout, BH_LAYOUT_TACTSUITX16_SIZE> bhLayout = { BH_LAYOUT_TACTSUITX16 };

// Ouput indices, responsible for x40 => x16 grouping
static const std::array<std::uint8_t,BH_LAYOUT_TACTSUITX16_GROUPS_SIZE> layoutGroups = BH_LAYOUT_TACTSUITX16_GROUPS;

void setupMode()
{
    // Configure the PCA9685
    auto* pwm = new Adafruit_PWMServoDriver(0x40);
    pwm->begin();
    pwm->setPWMFreq(PWM_FREQUENCY);

    // Assign the pins on the configured PCA9685 to positions on the vest
    auto frontOutputs = PlaneMapper_Margin::mapMatrixCoordinates<FloatPlane::Actuator>({
      // clang-format off
      { new PCA9685Output(pwm, 0), new PCA9685Output(pwm, 1), new PCA9685Output(pwm, 2), new PCA9685Output(pwm, 3) },
      { new PCA9685Output(pwm, 4), new PCA9685Output(pwm, 5), new PCA9685Output(pwm, 6), new PCA9685Output(pwm, 7) },
      // clang-format on
    });
    auto backOutputs = PlaneMapper_Margin::mapMatrixCoordinates<FloatPlane::Actuator>({
      // clang-format off
      { new PCA9685Output(pwm, 8),  new PCA9685Output(pwm, 9),  new PCA9685Output(pwm, 10), new PCA9685Output(pwm, 11) },
      { new PCA9685Output(pwm, 12), new PCA9685Output(pwm, 13), new PCA9685Output(pwm, 14), new PCA9685Output(pwm, 15) },
      // clang-format on
    });

    app->getVibroBody()->addTarget(Target::ChestFront, new FloatPlane_Closest(frontOutputs));
    app->getVibroBody()->addTarget(Target::ChestBack, new FloatPlane_Closest(backOutputs));

    app->getVibroBody()->setup();

    auto* bhBleConnection = new BLE::Connection(
      {
        .deviceName = BLUETOOTH_NAME,
        .appearance = BH_BLE_APPEARANCE,
        .serialNumber = BH_SERIAL_NUMBER,
      },
      [](std::string& value) -> void {
          Decoder::applyVestGrouped(app->getVibroBody(), value, bhLayout, layoutGroups);
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
