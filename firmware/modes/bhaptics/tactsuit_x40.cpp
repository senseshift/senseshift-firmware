#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include <Wire.h>

#include "connections/bhaptics.h"
#include "firmware.h"
#include "main.h"
#include "output_writers/pca9685.h"
#include "output_writers/direct.h"
#include "outputs/auto_margins.h"

// Front
// 0 1 30 31
// 2 3 32 33
// 4 5 34 35
// 6 7 36 37
// 8 9 38 39

// Back
// 10 11 20 21
// 12 13 22 23
// 14 15 24 25
// 16 17 26 27
// 18 19 28 29

const uint16_t _bh_max_x = 4;
const uint16_t _bh_max_y = 5;

inline Point2D* make_point(uint16_t x, uint16_t y) {
    return new Point2D(UINT16_MAX * (1 / ((float)_bh_max_x - 1)) * ((float)x), UINT16_MAX * (1 / ((float)_bh_max_y - 1)) * ((float)y));
}
Point2D* indexesToPoints[40] = {

    make_point(0, 0),
    make_point(1, 0),
    make_point(0, 1),
    make_point(1, 1),
    make_point(0, 2),
    make_point(1, 2),
    make_point(0, 3),
    make_point(1, 3),
    make_point(0, 4),
    make_point(1, 4),

    make_point(0, 0),
    make_point(1, 0),
    make_point(0, 1),
    make_point(1, 1),
    make_point(0, 2),
    make_point(1, 2),
    make_point(0, 3),
    make_point(1, 3),
    make_point(0, 4),
    make_point(1, 4),

    make_point(2, 0),
    make_point(3, 0),
    make_point(2, 1),
    make_point(3, 1),
    make_point(2, 2),
    make_point(3, 2),
    make_point(2, 3),
    make_point(3, 3),
    make_point(2, 4),
    make_point(3, 4),

    make_point(2, 0),
    make_point(3, 0),
    make_point(2, 1),
    make_point(3, 1),
    make_point(2, 2),
    make_point(3, 2),
    make_point(2, 3),
    make_point(3, 3),
    make_point(2, 4),
    make_point(3, 4)};

void vestMotorTransformer(std::string& value) {
    for (size_t i = 0; i < 20; i++) {
        uint8_t byte = value[i];
        uint actIndex = i * 2;

        outputData_t output_0;
        output_0.point = *indexesToPoints[actIndex];
        output_0.intensity = map(((byte >> 4) & 0xf), 0, 15, 0, UINT16_MAX);
        App.getOutput()->writeOutput(
            (actIndex < 10 || actIndex >= 30) ? OUTPUT_PATH_CHEST_FRONT : OUTPUT_PATH_CHEST_BACK,
            output_0);

        outputData_t output_1;
        output_1.point = *indexesToPoints[actIndex + 1];
        output_1.intensity = map((byte & 0xf), 0, 15, 0, UINT16_MAX);

        App.getOutput()->writeOutput(
            (actIndex < 10 || actIndex >= 30) ? OUTPUT_PATH_CHEST_FRONT : OUTPUT_PATH_CHEST_BACK,
            output_1);
    }
}

void setupMode() {
    Adafruit_PWMServoDriver* pwm1 = new Adafruit_PWMServoDriver(0x40);
    Adafruit_PWMServoDriver* pwm2 = new Adafruit_PWMServoDriver(0x41);
    pwm1->begin();
    pwm1->setPWMFreq(60);
    pwm2->begin();
    pwm2->setPWMFreq(60);

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

    ledcSetup(6, 60, 12);
    ledcAttachPin(12, 6);

    ledcSetup(7, 60, 12);
    ledcAttachPin(13, 7);

    autoOutputVector_t frontOutputs{
        {
            new PCA9685OutputWriter(pwm1, 0),
            new PCA9685OutputWriter(pwm1, 1),
            new PCA9685OutputWriter(pwm1, 2),
            new PCA9685OutputWriter(pwm1, 3),
        },
        {
            new PCA9685OutputWriter(pwm1, 4),
            new PCA9685OutputWriter(pwm1, 5),
            new PCA9685OutputWriter(pwm1, 6),
            new PCA9685OutputWriter(pwm1, 7),
        },
        {
            new PCA9685OutputWriter(pwm1, 8),
            new PCA9685OutputWriter(pwm1, 9),
            new PCA9685OutputWriter(pwm1, 10),
            new PCA9685OutputWriter(pwm1, 11),
        },
        {
            new PCA9685OutputWriter(pwm1, 12),
            new PCA9685OutputWriter(pwm1, 13),
            new PCA9685OutputWriter(pwm1, 14),
            new PCA9685OutputWriter(pwm1, 15),
        },
        {
            new DirectOutputWriter(0),
            new DirectOutputWriter(1),
            new DirectOutputWriter(2),
            new DirectOutputWriter(3),
        },
    };
    autoOutputVector_t backOutputs{
        {
            new PCA9685OutputWriter(pwm2, 0),
            new PCA9685OutputWriter(pwm2, 1),
            new PCA9685OutputWriter(pwm2, 2),
            new PCA9685OutputWriter(pwm2, 3),
        },
        {
            new PCA9685OutputWriter(pwm2, 4),
            new PCA9685OutputWriter(pwm2, 5),
            new PCA9685OutputWriter(pwm2, 6),
            new PCA9685OutputWriter(pwm2, 7),
        },
        {
            new PCA9685OutputWriter(pwm2, 8),
            new PCA9685OutputWriter(pwm2, 9),
            new PCA9685OutputWriter(pwm2, 10),
            new PCA9685OutputWriter(pwm2, 11),
        },
        {
            new PCA9685OutputWriter(pwm2, 12),
            new PCA9685OutputWriter(pwm2, 13),
            new PCA9685OutputWriter(pwm2, 14),
            new PCA9685OutputWriter(pwm2, 15),
        },
        {
            new DirectOutputWriter(4),
            new DirectOutputWriter(5),
            new DirectOutputWriter(6),
            new DirectOutputWriter(7),
        },
    };

    OutputAutoComponent_Margin* chestFront = new OutputAutoComponent_Margin(frontOutputs);
    OutputAutoComponent_Margin* chestBack = new OutputAutoComponent_Margin(backOutputs);

    App.getOutput()->addComponent(OUTPUT_PATH_CHEST_FRONT, chestFront);
    App.getOutput()->addComponent(OUTPUT_PATH_CHEST_BACK, chestBack);

    BHapticsBLEConnection* bhBleConnection = new BHapticsBLEConnection(BLUETOOTH_NAME, vestMotorTransformer);
    App.registerComponent(bhBleConnection);
}