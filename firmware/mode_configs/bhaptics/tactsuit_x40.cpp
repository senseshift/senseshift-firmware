// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include "senseshift.h"

#include <senseshift/arduino/input/sensor/analog.hpp>
#include <senseshift/arduino/output/ledc.hpp>
#include <senseshift/arduino/output/pca9685.hpp>
#include <senseshift/battery/sensor.hpp>
#include <senseshift/bh/ble/connection.hpp>
#include <senseshift/bh/devices.hpp>
#include <senseshift/bh/encoding.hpp>
#include <senseshift/freertos/input/sensor.hpp>

using namespace SenseShift;
using namespace SenseShift::Input;
using namespace SenseShift::Input::Filter;
using namespace SenseShift::Arduino::Output;
using namespace SenseShift::Arduino::Input;
using namespace SenseShift::FreeRTOS::Input;
using namespace SenseShift::Battery;
using namespace SenseShift::BH;
using namespace SenseShift::Body::Haptics;

extern Application App;
Application* app = &App;

static const std::array<OutputLayout, BH_LAYOUT_TACTSUITX40_SIZE> bhLayout = { BH_LAYOUT_TACTSUITX40 };

void setupMode()
{
    // Configure the PCA9685s
    auto* pwm0 = new Adafruit_PWMServoDriver(0x40);
    pwm0->begin();
    pwm0->setPWMFreq(PWM_FREQUENCY);

    auto* pwm1 = new Adafruit_PWMServoDriver(0x41);
    pwm1->begin();
    pwm1->setPWMFreq(PWM_FREQUENCY);

    // Assign the pins on the configured PCA9685s and PWM pins to locations on the
    // vest
    auto frontOutputs = PlaneMapper_Margin::mapMatrixCoordinates<FloatPlane::Actuator>({
      // clang-format off
      { new PCA9685Output(pwm0, 0),  new PCA9685Output(pwm0, 1),  new PCA9685Output(pwm0, 2),  new PCA9685Output(pwm0, 3)  },
      { new PCA9685Output(pwm0, 4),  new PCA9685Output(pwm0, 5),  new PCA9685Output(pwm0, 6),  new PCA9685Output(pwm0, 7)  },
      { new PCA9685Output(pwm0, 8),  new PCA9685Output(pwm0, 9),  new PCA9685Output(pwm0, 10), new PCA9685Output(pwm0, 11) },
      { new PCA9685Output(pwm0, 12), new PCA9685Output(pwm0, 13), new PCA9685Output(pwm0, 14), new PCA9685Output(pwm0, 15) },
      { new LedcOutput(32),          new LedcOutput(33),          new LedcOutput(25),          new LedcOutput(26)          },
      // clang-format on
    });
    auto backOutputs = PlaneMapper_Margin::mapMatrixCoordinates<FloatPlane::Actuator>({
      // clang-format off
      { new PCA9685Output(pwm1, 0),  new PCA9685Output(pwm1, 1),  new PCA9685Output(pwm1, 2),  new PCA9685Output(pwm1, 3)  },
      { new PCA9685Output(pwm1, 4),  new PCA9685Output(pwm1, 5),  new PCA9685Output(pwm1, 6),  new PCA9685Output(pwm1, 7)  },
      { new PCA9685Output(pwm1, 8),  new PCA9685Output(pwm1, 9),  new PCA9685Output(pwm1, 10), new PCA9685Output(pwm1, 11) },
      { new PCA9685Output(pwm1, 12), new PCA9685Output(pwm1, 13), new PCA9685Output(pwm1, 14), new PCA9685Output(pwm1, 15) },
      { new LedcOutput(27),          new LedcOutput(14),          new LedcOutput(12),          new LedcOutput(13)          },
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
          Decoder::applyVest(app->getVibroBody(), value, bhLayout);
      },
      app
    );
    bhBleConnection->begin();

#if defined(SENSESHIFT_BATTERY_ENABLED) && SENSESHIFT_BATTERY_ENABLED == true
    auto* batteryVoltageSensor = new SimpleSensorDecorator(new AnalogSimpleSensor(36));
    batteryVoltageSensor->addFilters({
      new MultiplyFilter(3.3F),                      // Convert to raw pin voltage
      new VoltageDividerFilter(27000.0F, 100000.0F), // Convert to voltage divider voltage
    });
    auto* batteryTask = new ::SenseShift::FreeRTOS::ComponentUpdateTask<SimpleSensorDecorator<float>>(
      batteryVoltageSensor,
      SENSESHIFT_BATTERY_SAMPLE_RATE,
      { "ADC Battery", 4096, SENSESHIFT_BATTERY_TASK_PRIORITY, tskNO_AFFINITY }
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
