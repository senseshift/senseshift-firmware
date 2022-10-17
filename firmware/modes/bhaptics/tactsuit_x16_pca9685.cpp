#include <Arduino.h>
#include <Wire.h>

#include "connections/bhaptics.h"
#include "firmware.h"
#include "main.h"
#include "output_writers/pca9685.h"
#include "outputs/auto_margins.h"

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
    make_point(1, 4),
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
    make_point(3, 4),
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
    make_point(3, 4)
};

void vestMotorTransformer(std::string& value) {
    for (size_t i = 0; i < 20; i++) {
        uint8_t byte = value[i];
        uint actIndex = i * 2;

        outputData_t output_0;
        output_0.point = *indexesToPoints[actIndex];
        output_0.intensity = map(((byte >> 4) & 0xf), 0, 15, 0, UINT16_MAX);

        App.getOutput()->writeOutput(
            (actIndex < 10 || actIndex >= 30) ? OUTPUT_PATH_CHEST_FRONT : OUTPUT_PATH_CHEST_BACK,
            output_0
        );

        outputData_t output_1;
        output_1.point = *indexesToPoints[actIndex + 1];
        output_1.intensity = map((byte & 0xf), 0, 15, 0, UINT16_MAX);

        App.getOutput()->writeOutput(
            (actIndex < 10 || actIndex >= 30) ? OUTPUT_PATH_CHEST_FRONT : OUTPUT_PATH_CHEST_BACK,
            output_1
        );
    }
}

void setupMode() {
    Adafruit_PWMServoDriver* pwm = new Adafruit_PWMServoDriver(0x40);
    pwm->begin();
    pwm->setPWMFreq(60);

    autoOutputVector_t frontOutputs{
        {
            new PCA9685OutputWriter(pwm, 0),
            new PCA9685OutputWriter(pwm, 1),
        },
        {
            new PCA9685OutputWriter(pwm, 2),
            new PCA9685OutputWriter(pwm, 3),
        },
        {
            new PCA9685OutputWriter(pwm, 4),
            new PCA9685OutputWriter(pwm, 5),
        },
        {
            new PCA9685OutputWriter(pwm, 6),
            new PCA9685OutputWriter(pwm, 7),
        },
    };
    autoOutputVector_t backOutputs{
        {
            new PCA9685OutputWriter(pwm, 8),
            new PCA9685OutputWriter(pwm, 9),
        },
        {
            new PCA9685OutputWriter(pwm, 10),
            new PCA9685OutputWriter(pwm, 11),
        },
        {
            new PCA9685OutputWriter(pwm, 12),
            new PCA9685OutputWriter(pwm, 13),
        },
        {
            new PCA9685OutputWriter(pwm, 14),
            new PCA9685OutputWriter(pwm, 15),
        },
    };

    OutputAutoComponent_Margin* chestFront = new OutputAutoComponent_Margin(frontOutputs);
    OutputAutoComponent_Margin* chestBack = new OutputAutoComponent_Margin(backOutputs);

    App.getOutput()->addComponent(OUTPUT_PATH_CHEST_FRONT, chestFront);
    App.getOutput()->addComponent(OUTPUT_PATH_CHEST_BACK, chestBack);

    BHapticsBLEConnection* bhBleConnection = new BHapticsBLEConnection(BLUETOOTH_NAME, vestMotorTransformer);
    App.registerComponent(bhBleConnection);
}