#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "firmware.h"
#include "main.h"
#include "connections/bhaptics.h"

#include "outputs/auto_margins.h"
#include "output_writers/pca9685.h"

const uint16_t _bh_step_x = UINT16_MAX / 4; // i-1
const uint16_t _bh_step_y = UINT16_MAX / 3; // j-1

Point2D* indexesToPoints[40] = {
    new Point2D(_bh_step_x * 0, _bh_step_y * 3), // 0
    new Point2D(_bh_step_x * 0, _bh_step_y * 2), // 1
    new Point2D(_bh_step_x * 1, _bh_step_y * 3), // 2
    new Point2D(_bh_step_x * 1, _bh_step_y * 2), // 3
    new Point2D(_bh_step_x * 2, _bh_step_y * 3), // 4
    new Point2D(_bh_step_x * 2, _bh_step_y * 2), // 5
    new Point2D(_bh_step_x * 3, _bh_step_y * 3), // 6
    new Point2D(_bh_step_x * 4, _bh_step_y * 2), // 7
    new Point2D(_bh_step_x * 4, _bh_step_y * 3), // 8
    new Point2D(_bh_step_x * 4, _bh_step_y * 2), // 9

    new Point2D(_bh_step_x * 0, _bh_step_y * 1), // 10
    new Point2D(_bh_step_x * 0, _bh_step_y * 0), // 11
    new Point2D(_bh_step_x * 1, _bh_step_y * 1), // 12
    new Point2D(_bh_step_x * 1, _bh_step_y * 0), // 13
    new Point2D(_bh_step_x * 2, _bh_step_y * 1), // 14
    new Point2D(_bh_step_x * 2, _bh_step_y * 0), // 15
    new Point2D(_bh_step_x * 3, _bh_step_y * 1), // 16
    new Point2D(_bh_step_x * 4, _bh_step_y * 0), // 17
    new Point2D(_bh_step_x * 4, _bh_step_y * 1), // 18
    new Point2D(_bh_step_x * 4, _bh_step_y * 0), // 19

    new Point2D(_bh_step_x * 0, _bh_step_y * 3), // 20
    new Point2D(_bh_step_x * 0, _bh_step_y * 2), // 21
    new Point2D(_bh_step_x * 1, _bh_step_y * 3), // 22
    new Point2D(_bh_step_x * 1, _bh_step_y * 2), // 23
    new Point2D(_bh_step_x * 2, _bh_step_y * 3), // 24
    new Point2D(_bh_step_x * 2, _bh_step_y * 2), // 25
    new Point2D(_bh_step_x * 3, _bh_step_y * 3), // 26
    new Point2D(_bh_step_x * 4, _bh_step_y * 2), // 27
    new Point2D(_bh_step_x * 4, _bh_step_y * 3), // 28
    new Point2D(_bh_step_x * 4, _bh_step_y * 2), // 29

    new Point2D(_bh_step_x * 0, _bh_step_y * 1), // 30
    new Point2D(_bh_step_x * 0, _bh_step_y * 0), // 31
    new Point2D(_bh_step_x * 1, _bh_step_y * 1), // 32
    new Point2D(_bh_step_x * 1, _bh_step_y * 0), // 33
    new Point2D(_bh_step_x * 2, _bh_step_y * 1), // 34
    new Point2D(_bh_step_x * 2, _bh_step_y * 0), // 35
    new Point2D(_bh_step_x * 3, _bh_step_y * 1), // 36
    new Point2D(_bh_step_x * 4, _bh_step_y * 0), // 37
    new Point2D(_bh_step_x * 4, _bh_step_y * 1), // 38
    new Point2D(_bh_step_x * 4, _bh_step_y * 0), // 39
};

void vestMotorTransformer(std::string &value)
{
    for (size_t i = 0; i < 20; i++) {
        uint8_t byte = value[i];
        uint actIndex = i*2;

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

void setupMode()
{
    Adafruit_PWMServoDriver* pwm = new Adafruit_PWMServoDriver(0x40);
    pwm->begin();
    pwm->setPWMFreq(60);

    autoOutputVector_t frontOutputs {
        { new PCA9685OutputWriter(pwm, 0), new PCA9685OutputWriter(pwm, 8), },
        { new PCA9685OutputWriter(pwm, 1), new PCA9685OutputWriter(pwm, 9), },
        { new PCA9685OutputWriter(pwm, 2), new PCA9685OutputWriter(pwm, 10), },
        { new PCA9685OutputWriter(pwm, 3), new PCA9685OutputWriter(pwm, 11), },
    };
    autoOutputVector_t backOutputs {
        { new PCA9685OutputWriter(pwm, 4), new PCA9685OutputWriter(pwm, 12) },
        { new PCA9685OutputWriter(pwm, 5), new PCA9685OutputWriter(pwm, 13) },
        { new PCA9685OutputWriter(pwm, 6), new PCA9685OutputWriter(pwm, 14) },
        { new PCA9685OutputWriter(pwm, 7), new PCA9685OutputWriter(pwm, 15) },
    };

    OutputAutoComponent_Margin* chestFront = new OutputAutoComponent_Margin(frontOutputs);
    // OutputAutoComponent_Margin* chestBack = new OutputAutoComponent_Margin(backOutputs);

    App.getOutput()->addComponent(OUTPUT_PATH_CHEST_FRONT, chestFront);
    // App.getOutput()->addComponent(OUTPUT_PATH_CHEST_BACK, chestBack);

    BHapticsBLEConnection* bhBleConnection = new BHapticsBLEConnection(BLUETOOTH_NAME, vestMotorTransformer);
    App.registerComponent(bhBleConnection);
}