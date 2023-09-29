// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include "senseshift.h"

#include <senseshift/arduino/input/sensor/analog.hpp>
#include <senseshift/arduino/output/actuator/pca9685.hpp>
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

static constexpr size_t bhLayoutSize = BH_LAYOUT_TACTSUITX40_SIZE;
static const OutputLayout bhLayout[bhLayoutSize] = BH_LAYOUT_TACTSUITX40;

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
    auto frontOutputs = PlaneMapper_Margin::mapMatrixCoordinates<VibroPlane::Actuator>({
      // clang-format off
      { new ActuatorPCA9685(pwm0, 0),  new ActuatorPCA9685(pwm0, 1),  new ActuatorPCA9685(pwm0, 2),  new ActuatorPCA9685(pwm0, 3)  },
      { new ActuatorPCA9685(pwm0, 4),  new ActuatorPCA9685(pwm0, 5),  new ActuatorPCA9685(pwm0, 6),  new ActuatorPCA9685(pwm0, 7)  },
      { new ActuatorPCA9685(pwm0, 8),  new ActuatorPCA9685(pwm0, 9),  new ActuatorPCA9685(pwm0, 10), new ActuatorPCA9685(pwm0, 11) },
      { new ActuatorPCA9685(pwm0, 12), new ActuatorPCA9685(pwm0, 13), new ActuatorPCA9685(pwm0, 14), new ActuatorPCA9685(pwm0, 15) },
      { new ActuatorPWM(32),           new ActuatorPWM(33),           new ActuatorPWM(25),           new ActuatorPWM(26)           },
      // clang-format on
    });
    auto backOutputs = PlaneMapper_Margin::mapMatrixCoordinates<VibroPlane::Actuator>({
      // clang-format off
      { new ActuatorPCA9685(pwm1, 0),  new ActuatorPCA9685(pwm1, 1),  new ActuatorPCA9685(pwm1, 2),  new ActuatorPCA9685(pwm1, 3)  },
      { new ActuatorPCA9685(pwm1, 4),  new ActuatorPCA9685(pwm1, 5),  new ActuatorPCA9685(pwm1, 6),  new ActuatorPCA9685(pwm1, 7)  },
      { new ActuatorPCA9685(pwm1, 8),  new ActuatorPCA9685(pwm1, 9),  new ActuatorPCA9685(pwm1, 10), new ActuatorPCA9685(pwm1, 11) },
      { new ActuatorPCA9685(pwm1, 12), new ActuatorPCA9685(pwm1, 13), new ActuatorPCA9685(pwm1, 14), new ActuatorPCA9685(pwm1, 15) },
      { new ActuatorPWM(27),           new ActuatorPWM(14),           new ActuatorPWM(12),           new ActuatorPWM(13)           },
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
          Decoder::applyVest(app->getHapticBody(), value, bhLayout);
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
