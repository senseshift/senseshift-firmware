// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include "I2CDevLib.h"
#include "i2cdev/pca9685.hpp"

#include "senseshift.h"

#include <senseshift/arduino/input/sensor/analog.hpp>
#include <senseshift/arduino/output/ledc.hpp>
#include <senseshift/battery/input/battery_sensor.hpp>
#include <senseshift/bh/ble/connection.hpp>
#include <senseshift/bh/devices.hpp>
#include <senseshift/bh/encoding.hpp>
#include <senseshift/freertos/task.hpp>
#include <senseshift/output/i2cdevlib_pwm.hpp>

using namespace SenseShift;
using namespace SenseShift::Input;
using namespace SenseShift::Input::Filter;
using namespace SenseShift::Arduino::Output;
using namespace SenseShift::Output;
using namespace SenseShift::Arduino::Input;
using namespace SenseShift::Battery;
using namespace SenseShift::Battery::Input;
using namespace SenseShift::BH;
using namespace SenseShift::Body::Haptics;

Application App;
Application* app = &App;

static const std::array<OutputLayout, BH_LAYOUT_TACTSUITX40_SIZE> bhLayout = { BH_LAYOUT_TACTSUITX40 };

void setup()
{
    Wire.begin();

    // Configure the PCA9685s
    auto pwm0 = i2cdev::PCA9685(0x40, I2CDev);
    if (pwm0.setFrequency(PWM_FREQUENCY) != I2CDEV_RESULT_OK) {
        LOG_E("pca9685", "Failed to set frequency");
    }
    if (pwm0.wakeup() != I2CDEV_RESULT_OK) {
        LOG_E("pca9685", "Failed to wake up");
    }

    auto pwm1 = i2cdev::PCA9685(0x41, I2CDev);
    if (pwm1.setFrequency(PWM_FREQUENCY) != I2CDEV_RESULT_OK) {
        LOG_E("pca9685", "Failed to set frequency");
    }
    if (pwm1.wakeup() != I2CDEV_RESULT_OK) {
        LOG_E("pca9685", "Failed to wake up");
    }

    // Assign the pins on the configured PCA9685s and PWM pins to locations on the
    // vest
    auto frontOutputs = PlaneMapper_Margin::mapMatrixCoordinates<FloatPlane::Actuator*>({
      // clang-format off
          { new PCA9685Output(pwm0, 0),  new PCA9685Output(pwm0, 1),  new PCA9685Output(pwm0, 2),  new PCA9685Output(pwm0, 3)  },
          { new PCA9685Output(pwm0, 4),  new PCA9685Output(pwm0, 5),  new PCA9685Output(pwm0, 6),  new PCA9685Output(pwm0, 7)  },
          { new PCA9685Output(pwm0, 8),  new PCA9685Output(pwm0, 9),  new PCA9685Output(pwm0, 10), new PCA9685Output(pwm0, 11) },
          { new PCA9685Output(pwm0, 12), new PCA9685Output(pwm0, 13), new PCA9685Output(pwm0, 14), new PCA9685Output(pwm0, 15) },
          { new LedcOutput(32),          new LedcOutput(33),          new LedcOutput(25),          new LedcOutput(26)          },
      // clang-format on
    });
    auto backOutputs = PlaneMapper_Margin::mapMatrixCoordinates<FloatPlane::Actuator*>({
      // clang-format off
          { new PCA9685Output(pwm1, 0),  new PCA9685Output(pwm1, 1),  new PCA9685Output(pwm1, 2),  new PCA9685Output(pwm1, 3)  },
          { new PCA9685Output(pwm1, 4),  new PCA9685Output(pwm1, 5),  new PCA9685Output(pwm1, 6),  new PCA9685Output(pwm1, 7)  },
          { new PCA9685Output(pwm1, 8),  new PCA9685Output(pwm1, 9),  new PCA9685Output(pwm1, 10), new PCA9685Output(pwm1, 11) },
          { new PCA9685Output(pwm1, 12), new PCA9685Output(pwm1, 13), new PCA9685Output(pwm1, 14), new PCA9685Output(pwm1, 15) },
          { new LedcOutput(27),          new LedcOutput(14),          new LedcOutput(12),          new LedcOutput(13)          },
      // clang-format on
    });

    app->getVibroBody()->addTarget(Target::ChestFront, new FloatPlane(frontOutputs));
    app->getVibroBody()->addTarget(Target::ChestBack, new FloatPlane(backOutputs));

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

#if defined(SS_BATTERY_ENABLED) && SS_BATTERY_ENABLED == true
    auto* batteryVoltageSensor = new SimpleSensorDecorator(new AnalogSimpleSensor(36));
    batteryVoltageSensor->addFilters({
      new MultiplyFilter(3.3F),                      // Convert to raw pin voltage
      new VoltageDividerFilter(27000.0F, 100000.0F), // Convert to voltage divider voltage
    });
    auto* batteryTask = new ::SenseShift::FreeRTOS::ComponentUpdateTask(
      batteryVoltageSensor,
      SS_BATTERY_SAMPLE_RATE,
      { "ADC Battery", 4096, SS_BATTERY_TASK_PRIORITY, tskNO_AFFINITY }
    );
    batteryTask->begin();

    auto* batterySensor = new LookupTableInterpolateBatterySensor(batteryVoltageSensor, &VoltageMap::LiPO_1S_42);
    batterySensor->addValueCallback([](BatteryState value) -> void {
        app->postEvent(new BatteryLevelEvent(value));
    });
    batterySensor->init();

//    i2cdev::MAX17048 gauge = i2cdev::MAX17048(MAX1704X_I2CADDR_BASE, I2CDev);
//    if (gauge.check() != I2CDEV_RESULT_OK) {
//        LOG_E("MAX17048", "Failed to initialize");
//    }
//    if (gauge.quickStart() != I2CDEV_RESULT_OK) {
//        LOG_E("MAX17048", "Failed to quick start");
//    }
//
//    IBatterySensor* batterySensor = new SimpleSensorDecorator(new MAX170XXBatterySensor(gauge));
//    batterySensor->addValueCallback([](BatteryState value) -> void {
//        app->postEvent(new BatteryLevelEvent(value));
//    });
//    batterySensor->init();
#endif
}

void loop()
{
    // Free up the Arduino loop task
    vTaskDelete(NULL);
}
