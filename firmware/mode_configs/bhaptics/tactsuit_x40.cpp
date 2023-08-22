// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include "senseshift.h"

#include <senseshift/arduino/input/sensor/analog.hpp>
#include <senseshift/arduino/output/pca9685.hpp>
#include <senseshift/arduino/output/pwm.hpp>
#include <senseshift/battery/sensor.hpp>
#include <senseshift/bh/ble/connection.hpp>
#include <senseshift/bh/devices.hpp>
#include <senseshift/bh/encoding.hpp>

using namespace SenseShift;
using namespace SenseShift::Arduino::Output;
using namespace SenseShift::Arduino::Input;
using namespace SenseShift::Battery;
using namespace SenseShift::BH;
using namespace SenseShift::Body::Haptics;

extern SenseShift::SenseShift App;
SenseShift::SenseShift* app = &App;

static const size_t bhLayoutSize = BH_LAYOUT_TACTSUITX40_SIZE;
static const OutputLayout_t bhLayout[bhLayoutSize] = BH_LAYOUT_TACTSUITX40;

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
    auto frontOutputs = PlaneMapper_Margin::mapMatrixCoordinates<VibroPlane::Actuator_t>({
      // clang-format off
      { new PCA9685OutputWriter(pwm0, 0),  new PCA9685OutputWriter(pwm0, 1),  new PCA9685OutputWriter(pwm0, 2),  new PCA9685OutputWriter(pwm0, 3)  },
      { new PCA9685OutputWriter(pwm0, 4),  new PCA9685OutputWriter(pwm0, 5),  new PCA9685OutputWriter(pwm0, 6),  new PCA9685OutputWriter(pwm0, 7)  },
      { new PCA9685OutputWriter(pwm0, 8),  new PCA9685OutputWriter(pwm0, 9),  new PCA9685OutputWriter(pwm0, 10), new PCA9685OutputWriter(pwm0, 11) },
      { new PCA9685OutputWriter(pwm0, 12), new PCA9685OutputWriter(pwm0, 13), new PCA9685OutputWriter(pwm0, 14), new PCA9685OutputWriter(pwm0, 15) },
      { new PWMOutputWriter(32),           new PWMOutputWriter(33),           new PWMOutputWriter(25),           new PWMOutputWriter(26)           },
      // clang-format on
    });
    auto backOutputs = PlaneMapper_Margin::mapMatrixCoordinates<VibroPlane::Actuator_t>({
      // clang-format off
      { new PCA9685OutputWriter(pwm1, 0),  new PCA9685OutputWriter(pwm1, 1),  new PCA9685OutputWriter(pwm1, 2),  new PCA9685OutputWriter(pwm1, 3)  },
      { new PCA9685OutputWriter(pwm1, 4),  new PCA9685OutputWriter(pwm1, 5),  new PCA9685OutputWriter(pwm1, 6),  new PCA9685OutputWriter(pwm1, 7)  },
      { new PCA9685OutputWriter(pwm1, 8),  new PCA9685OutputWriter(pwm1, 9),  new PCA9685OutputWriter(pwm1, 10), new PCA9685OutputWriter(pwm1, 11) },
      { new PCA9685OutputWriter(pwm1, 12), new PCA9685OutputWriter(pwm1, 13), new PCA9685OutputWriter(pwm1, 14), new PCA9685OutputWriter(pwm1, 15) },
      { new PWMOutputWriter(27),           new PWMOutputWriter(14),           new PWMOutputWriter(12),           new PWMOutputWriter(13)           },
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
