#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include <Wire.h>

#include "connections/bhaptics.h"
#include "firmware.h"
#include "main.h"
#include "output_writers/direct.h"
#include "output_writers/pca9685.h"
#include "outputs/auto_margins.h"

const uint16_t _bh_max_x = 3;

inline Point2D* make_point(uint16_t x) {
    return new Point2D(UINT16_MAX * (1 / ((float)_bh_max_x - 1)) * ((float)x), 0);
}

Point2D* indexesToPoints[_bh_max_x] = {
    make_point(0),
    make_point(1),
    make_point(2)
};

void vestMotorTransformer(std::string& value) {
    for (size_t i = 0; i < _bh_max_x; i++) {
        uint8_t byte = value[i];
        outputData_t output_0;
        output_0.point = *indexesToPoints[i];
        output_0.intensity = map(byte, 0, 100, 0, UINT16_MAX);
        App.getOutput()->writeOutput(
            OUTPUT_PATH_ACCESSORY,
            output_0);
    }
}

void setupMode() {
    ledcSetup(0, 60, 12);
    ledcAttachPin(32, 0);

    ledcSetup(1, 60, 12);
    ledcAttachPin(33, 1);

    ledcSetup(2, 60, 12);
    ledcAttachPin(25, 2);

    autoOutputVector_t frontOutputs{
        {
            new DirectOutputWriter(0),
            new DirectOutputWriter(1),
            new DirectOutputWriter(2),
        }};

    OutputAutoComponent_Margin* foot = new OutputAutoComponent_Margin(frontOutputs);

    App.getOutput()->addComponent(OUTPUT_PATH_ACCESSORY, foot);

    BHapticsBLEConnection* bhBleConnection = new BHapticsBLEConnection(BLUETOOTH_NAME, vestMotorTransformer);
    App.registerComponent(bhBleConnection);
}