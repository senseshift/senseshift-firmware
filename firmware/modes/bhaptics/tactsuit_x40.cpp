#include <Arduino.h>
#include <Wire.h>

#include "openhaptics.h"
#include "utils.h"
#include "auto_output.h"

#include "connections/bhaptics.h"
#include "output_components/closest.h"
#include "output_writers/pca9685.h"
#include "output_writers/ledc.h"

#define PWM_FREQUENCY 60
#define PWM_RESOLUTION 12

#pragma region bHaptics_trash

const uint16_t _bh_max_x = 4;
const uint16_t _bh_max_y = 5;

inline Point2D* make_point(uint16_t x, uint16_t y) {
    return getPoint(x, y, _bh_max_x, _bh_max_y);
}

Point2D* indexesToPoints[40] = {
    // Front, left part
    /*  0 */ make_point(0, 0),
    /*  1 */ make_point(1, 0),
    /*  2 */ make_point(0, 1),
    /*  3 */ make_point(1, 1),
    /*  4 */ make_point(0, 2),
    /*  5 */ make_point(1, 2),
    /*  6 */ make_point(0, 3),
    /*  7 */ make_point(1, 3),
    /*  8 */ make_point(0, 4),
    /*  9 */ make_point(1, 4),

    // Back
    /* 11 */ make_point(0, 0),
    /* 11 */ make_point(1, 0),
    /* 12 */ make_point(0, 1),
    /* 13 */ make_point(1, 1),
    /* 14 */ make_point(0, 2),
    /* 15 */ make_point(1, 2),
    /* 16 */ make_point(0, 3),
    /* 17 */ make_point(1, 3),
    /* 18 */ make_point(0, 4),
    /* 19 */ make_point(1, 4),

    /* 20 */ make_point(2, 0),
    /* 21 */ make_point(3, 0),
    /* 22 */ make_point(2, 1),
    /* 23 */ make_point(3, 1),
    /* 24 */ make_point(2, 2),
    /* 25 */ make_point(3, 2),
    /* 26 */ make_point(2, 3),
    /* 27 */ make_point(3, 3),
    /* 28 */ make_point(2, 4),
    /* 29 */ make_point(3, 4),

    // Front, again... Now right part
    /* 30 */ make_point(2, 0),
    /* 31 */ make_point(3, 0),
    /* 32 */ make_point(2, 1),
    /* 33 */ make_point(3, 1),
    /* 34 */ make_point(2, 2),
    /* 35 */ make_point(3, 2),
    /* 36 */ make_point(2, 3),
    /* 37 */ make_point(3, 3),
    /* 38 */ make_point(2, 4),
    /* 39 */ make_point(3, 4)
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

#pragma endregion bHaptics_trash

void setupMode() {
    // Configure the PCA9685s
    Adafruit_PWMServoDriver* pwm1 = new Adafruit_PWMServoDriver(0x40);
    pwm1->begin();
    pwm1->setPWMFreq(PWM_FREQUENCY);

    Adafruit_PWMServoDriver* pwm2 = new Adafruit_PWMServoDriver(0x41);
    pwm2->begin();
    pwm2->setPWMFreq(PWM_FREQUENCY);

        // Configure PWM channels, and attach them to pins
    // TODO: decide on better way to setup PWM pins
    ledcSetup(0, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(32, 0);

    ledcSetup(1, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(33, 1);

    ledcSetup(2, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(25, 2);

    ledcSetup(3, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(26, 3);

    ledcSetup(4, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(27, 4);

    ledcSetup(5, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(14, 5);

    ledcSetup(6, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(12, 6);

    ledcSetup(7, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(13, 7);

    // Assign the pins on the configured PCA9685s and PWM channels to locations on the vest
    auto frontOutputs = transformAutoOutput({
        { new PCA9685OutputWriter(pwm1, 0),  new PCA9685OutputWriter(pwm1, 1),  new PCA9685OutputWriter(pwm1, 2),  new PCA9685OutputWriter(pwm1, 3)  },
        { new PCA9685OutputWriter(pwm1, 4),  new PCA9685OutputWriter(pwm1, 5),  new PCA9685OutputWriter(pwm1, 6),  new PCA9685OutputWriter(pwm1, 7)  },
        { new PCA9685OutputWriter(pwm1, 8),  new PCA9685OutputWriter(pwm1, 9),  new PCA9685OutputWriter(pwm1, 10), new PCA9685OutputWriter(pwm1, 11) },
        { new PCA9685OutputWriter(pwm1, 12), new PCA9685OutputWriter(pwm1, 13), new PCA9685OutputWriter(pwm1, 14), new PCA9685OutputWriter(pwm1, 15) },
        { new LEDCOutputWriter(0),           new LEDCOutputWriter(1),           new LEDCOutputWriter(2),           new LEDCOutputWriter(3)           },
    });
    auto backOutputs = transformAutoOutput({
        { new PCA9685OutputWriter(pwm2, 0),  new PCA9685OutputWriter(pwm2, 1),  new PCA9685OutputWriter(pwm2, 2),  new PCA9685OutputWriter(pwm2, 3)  },
        { new PCA9685OutputWriter(pwm2, 4),  new PCA9685OutputWriter(pwm2, 5),  new PCA9685OutputWriter(pwm2, 6),  new PCA9685OutputWriter(pwm2, 7)  },
        { new PCA9685OutputWriter(pwm2, 8),  new PCA9685OutputWriter(pwm2, 9),  new PCA9685OutputWriter(pwm2, 10), new PCA9685OutputWriter(pwm2, 11) },
        { new PCA9685OutputWriter(pwm2, 12), new PCA9685OutputWriter(pwm2, 13), new PCA9685OutputWriter(pwm2, 14), new PCA9685OutputWriter(pwm2, 15) },
        { new LEDCOutputWriter(4),           new LEDCOutputWriter(5),           new LEDCOutputWriter(6),           new LEDCOutputWriter(7)           },
    });

    auto chestFront = new ClosestOutputComponent(frontOutputs);
    auto chestBack = new ClosestOutputComponent(backOutputs);

    App.getOutput()->addComponent(OUTPUT_PATH_CHEST_FRONT, chestFront);
    App.getOutput()->addComponent(OUTPUT_PATH_CHEST_BACK, chestBack);

    BHapticsBLEConnection* bhBleConnection = new BHapticsBLEConnection(BLUETOOTH_NAME, vestMotorTransformer);
    App.setConnection(bhBleConnection);
}