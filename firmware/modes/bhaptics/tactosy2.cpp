#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include <Wire.h>

#include "connections/bhaptics.h"
#include "firmware.h"
#include "main.h"
#include "output_writers/ledc.h"
#include "output_writers/pca9685.h"
#include "outputs/auto_margins.h"

const uint16_t _bh_max_x = 3;
const uint16_t _bh_max_y = 2;

inline Point2D* make_point(uint16_t x, uint16_t y) {
    return new Point2D(UINT16_MAX * (1 / ((float)_bh_max_x - 1)) * ((float)x), UINT16_MAX * (1 / ((float)_bh_max_y - 1)) * ((float)y));
}

Point2D* indexesToPoints[_bh_max_x * _bh_max_y] = {
    make_point(0, 0),
    make_point(1, 0),
    make_point(2, 0),
    make_point(0, 1),
    make_point(1, 1),
    make_point(2, 1)
};

void vestMotorTransformer(std::string& value) {
    for (size_t i = 0; i < (_bh_max_x * _bh_max_y); i++) {
        uint8_t byte = value[i];
        outputData_t output_0;
        output_0.point = *indexesToPoints[i];
        output_0.intensity = map(byte, 0, 100, 0, UINT16_MAX);
        App.getOutput()->writeOutput(OUTPUT_PATH_ACCESSORY, output_0);
    }
}

void setupMode() {
    
    // Configure PWM channels, and attach them to pins
    ledcSetup(0, 60, 12);
    ledcAttachPin(32, 0);

    ledcSetup(1, 60, 12);
    ledcAttachPin(33, 1);

    ledcSetup(2, 60, 12);
    ledcAttachPin(25, 2);

    ledcSetup(3, 60, 12);
    ledcAttachPin(26, 3);

    ledcSetup(4, 60, 12);
    ledcAttachPin(27, 4);

    ledcSetup(5, 60, 12);
    ledcAttachPin(14, 5);

    // Map the above channels to their positions on the forearm
    autoOutputVector_t forearmOutputs{
        { new LEDCOutputWriter(0), new LEDCOutputWriter(1), new LEDCOutputWriter(2) },
        { new LEDCOutputWriter(3), new LEDCOutputWriter(4), new LEDCOutputWriter(5) }
    };

    OutputAutoComponent_Margin* forearm = new OutputAutoComponent_Margin(forearmOutputs);

    App.getOutput()->addComponent(OUTPUT_PATH_ACCESSORY, forearm);

    BHapticsBLEConnection* bhBleConnection = new BHapticsBLEConnection(BLUETOOTH_NAME, vestMotorTransformer);
    App.registerComponent(bhBleConnection);
}