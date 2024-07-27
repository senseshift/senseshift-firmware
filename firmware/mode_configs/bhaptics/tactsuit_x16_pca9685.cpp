// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include <I2CDevLib.h>
#include <i2cdev/pca9685.hpp>

#include <senseshift.h>

#include <senseshift/arduino/input/sensor/analog.hpp>
#include <senseshift/battery/input/battery_sensor.hpp>
#include <senseshift/bh/ble/connection.hpp>
#include <senseshift/bh/devices.hpp>
#include <senseshift/bh/encoding.hpp>
#include <senseshift/freertos/task.hpp>
#include <senseshift/output/i2cdevlib_pwm.hpp>

using namespace SenseShift;
using namespace SenseShift::Input;
using namespace SenseShift::Input::Filter;
using namespace SenseShift::Output;
using namespace SenseShift::Arduino::Input;
using namespace SenseShift::Battery;
using namespace SenseShift::Battery::Input;
using namespace SenseShift::BH;
using namespace SenseShift::Body::Haptics;

extern Application App;
Application* app = &App;

static const std::array<OutputLayout, BH_LAYOUT_TACTSUITX16_SIZE> bhLayout = { BH_LAYOUT_TACTSUITX16 };

// Ouput indices, responsible for x40 => x16 grouping
static const std::array<std::uint8_t, BH_LAYOUT_TACTSUITX16_GROUPS_SIZE> layoutGroups = BH_LAYOUT_TACTSUITX16_GROUPS;

void setupMode()
{
    Wire.begin();

    // Configure the PCA9685s
    auto pwm = i2cdev::PCA9685(0x40, I2CDev);
    pwm.setFrequency(PWM_FREQUENCY);
    pwm.wakeup();

    // Assign the pins on the configured PCA9685 to positions on the vest
    auto frontOutputs = PlaneMapper_Margin::mapMatrixCoordinates<FloatPlane::Actuator*>({
      // clang-format off
      { new PCA9685Output(pwm, 0), new PCA9685Output(pwm, 1), new PCA9685Output(pwm, 2), new PCA9685Output(pwm, 3) },
      { new PCA9685Output(pwm, 4), new PCA9685Output(pwm, 5), new PCA9685Output(pwm, 6), new PCA9685Output(pwm, 7) },
      // clang-format on
    });
    auto backOutputs = PlaneMapper_Margin::mapMatrixCoordinates<FloatPlane::Actuator*>({
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

#if defined(SS_BATTERY_ENABLED) && SS_BATTERY_ENABLED == true
    auto* batteryVoltageSensor = new SimpleSensorDecorator(new AnalogSimpleSensor(36));
    batteryVoltageSensor->addFilters({
      new MultiplyFilter(3.3F),                      // Convert to raw pin voltage
      new VoltageDividerFilter(27000.0F, 100000.0F), // Convert to voltage divider voltage
    });
    auto* batteryTask = new ::SenseShift::FreeRTOS::ComponentUpdateTask<SimpleSensorDecorator<float>>(
      batteryVoltageSensor,
      SS_BATTERY_SAMPLE_RATE,
      { "ADC Battery", 4096, SS_BATTERY_TASK_PRIORITY, tskNO_AFFINITY }
    );
    batteryTask->begin();

    auto* batterySensor = new LookupTableInterpolateBatterySensor<const frozen::map<float, float, 21>>(
      batteryVoltageSensor,
      &VoltageMap::LiPO_1S_42
    );
    batterySensor->addValueCallback([](BatteryState value) -> void {
        app->postEvent(new BatteryLevelEvent(value));
    });
    batterySensor->init();
#endif
}

void loopMode()
{
    // Free up the Arduino loop task
    vTaskDelete(NULL);
}
