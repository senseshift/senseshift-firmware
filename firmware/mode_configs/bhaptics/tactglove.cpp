// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include <utility.hpp>

#include "senseshift.h"

#include <bh_encoding.hpp>
#include <bh_devices.hpp>
#include <connection_bhble.hpp>
#include <output_writers/pwm.hpp>

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include <battery/adc_naive.hpp>
#endif

#ifndef BH_LAYOUT
#define BH_LAYOUT BH::TactGloveLeftLayout
#endif

using namespace OH;
using namespace SenseShift;
using namespace SenseShift::Body::Haptics;
using namespace BH;

extern SenseShift::SenseShift App;
SenseShift::SenseShift* app = &App;

const auto& bhLayout = BH_LAYOUT;

void setupMode()
{
    // Configure PWM pins to their positions on the glove
    auto* motorThumb = new PWMOutputWriter(33); // Thumb
    auto* motorIndex = new PWMOutputWriter(32); // Index
    auto* motorMiddle = new PWMOutputWriter(25); // Middle
    auto* motorRing = new PWMOutputWriter(26); // Ring
    auto* motorLittle = new PWMOutputWriter(27); // Little
    auto* motorWrist = new PWMOutputWriter(14); // Wrist

    app->getHapticBody()->addTarget(std::get<0>(bhLayout[0]), new VibroPlane({{ std::get<1>(bhLayout[0]), motorThumb }}));
    app->getHapticBody()->addTarget(std::get<0>(bhLayout[1]), new VibroPlane({{ std::get<1>(bhLayout[1]), motorIndex }}));
    app->getHapticBody()->addTarget(std::get<0>(bhLayout[2]), new VibroPlane({{ std::get<1>(bhLayout[2]), motorMiddle }}));
    app->getHapticBody()->addTarget(std::get<0>(bhLayout[3]), new VibroPlane({{ std::get<1>(bhLayout[3]), motorRing }}));
    app->getHapticBody()->addTarget(std::get<0>(bhLayout[4]), new VibroPlane({{ std::get<1>(bhLayout[4]), motorLittle }}));
    app->getHapticBody()->addTarget(std::get<0>(bhLayout[5]), new VibroPlane({{ std::get<1>(bhLayout[5]), motorWrist }}));

    app->getHapticBody()->setup();

    uint8_t serialNumber[BH_SERIAL_NUMBER_LENGTH] = BH_SERIAL_NUMBER;
    ConnectionBHBLE_Config config = {
        .deviceName = BLUETOOTH_NAME,
        .appearance = BH_BLE_APPEARANCE,
        .serialNumber = serialNumber,
    };
    auto* bhBleConnection = new ConnectionBHBLE(
      config,
      [](std::string& value) -> void {
        Decoder::applyPlain(app->getHapticBody(), value, bhLayout, Effect_t::Vibro);
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
