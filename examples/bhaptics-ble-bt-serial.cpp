// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include "senseshift.h"

#include <bh_utils.hpp>
#include <connection_bhble.hpp>
#include <output_writers/pwm.hpp>

#include <BluetoothSerial.h>
#include <HardwareSerial.h>

using namespace BH;

extern SenseShift App;
SenseShift* app = &App;

BluetoothSerial SerialBT;
BluetoothSerial* btSerial = &SerialBT;

static const size_t bhLayoutSize = BH_LAYOUT_TACTAL_SIZE;
static const ::SenseShift::Body::Haptics::Position_t* bhLayout[bhLayoutSize] = BH_LAYOUT_TACTAL;

class BLECallbacks : public BHBLEConnectionCallbacks {
  public:
    void postInit() { btSerial->begin("SenseShift Serial"); }
};

void setupMode()
{
    // Configure PWM pins to their positions on the face
    auto faceOutputs = PlaneMapper_Margin::mapMatrixCoordinates<AbstractActuator>({
      // clang-format off
      {new PWMOutputWriter(32), new PWMOutputWriter(33), new PWMOutputWriter(25), new PWMOutputWriter(26), new PWMOutputWriter(27), new PWMOutputWriter(14)},
      // clang-format on
    });

    auto* face = new HapticPlane_Closest(faceOutputs);
    app->getHapticBody()->addComponent(OUTPUT_PATH_ACCESSORY, face);

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
          plainOutputTransformer(app->getHapticBody(), value, bhLayout, bhLayoutSize, OUTPUT_PATH_ACCESSORY);
      },
      app
    );
    bhBleConnection->setCallbacks(new BLECallbacks());
    bhBleConnection->begin();
}

void loopMode()
{
    // This way is suboptimal, but hardware interrupts for Serial are not supported by the Arduino framework
    if (btSerial->available()) {
        Serial.print(btSerial->read());
    }
    if (Serial.available()) {
        btSerial->write(Serial.read());
    }
    sleep(20);
}
