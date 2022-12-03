#include <Arduino.h>
#include <Wire.h>

// Override you configs below

// Experimental battery support
#define BATTERY_ENABLED false

// Stop overriding your configs

#include "config/all.h"

#include "openhaptics.h"
#include "utils.h"
#include "auto_output.h"

#include "connections/bhaptics.h"
#include "output_components/closest.h"
#include "output_writers/pca9685.h"

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
    #include "battery/abstract_battery.h"
    #include "battery/adc_battery.h"
#endif

#define PWM_FREQUENCY 60

#pragma region bHaptics_trash

const uint16_t _bh_max_x = 4;
const uint16_t _bh_max_y = 2;

inline Point2D* make_point(uint16_t x, uint16_t y) {
    return getPoint(x, y, _bh_max_x, _bh_max_y);
}

Point2D* indexesToPoints[40] = {
    // Front, left part
    /*  0 */ make_point(0, 0), // 0
    /*  1 */ make_point(1, 0), // 1
    /*  2 */ make_point(0, 0), // 4
    /*  3 */ make_point(1, 0), // 5

    /*  4 */ make_point(0, 1), // 8
    /*  5 */ make_point(1, 1), // 9
    /*  6 */ make_point(0, 1), // 12
    /*  7 */ make_point(1, 1), // 13
    /*  8 */ make_point(0, 1), // 16
    /*  9 */ make_point(1, 1), // 17

    // Back
    /* 10 */ make_point(0, 0), // 0
    /* 11 */ make_point(1, 0), // 1
    /* 12 */ make_point(0, 0), // 4
    /* 13 */ make_point(1, 0), // 5

    /* 14 */ make_point(0, 1), // 8
    /* 15 */ make_point(1, 1), // 9
    /* 16 */ make_point(0, 1), // 12
    /* 17 */ make_point(1, 1), // 13
    /* 18 */ make_point(0, 1), // 16
    /* 19 */ make_point(1, 1), // 17

    /* 20 */ make_point(2, 0), // 2
    /* 21 */ make_point(3, 0), // 3
    /* 22 */ make_point(2, 0), // 4
    /* 23 */ make_point(3, 0), // 7

    /* 24 */ make_point(2, 1), // 10
    /* 25 */ make_point(3, 1), // 11
    /* 26 */ make_point(2, 1), // 14
    /* 27 */ make_point(3, 1), // 15
    /* 28 */ make_point(2, 1), // 18
    /* 29 */ make_point(3, 1), // 19

    // Front, again... Now right part
    /* 30 */ make_point(2, 0), // 2
    /* 31 */ make_point(3, 0), // 3
    /* 32 */ make_point(2, 0), // 4
    /* 33 */ make_point(3, 0), // 7

    /* 34 */ make_point(2, 1), // 10
    /* 35 */ make_point(3, 1), // 11
    /* 36 */ make_point(2, 1), // 14
    /* 37 */ make_point(3, 1), // 15
    /* 38 */ make_point(2, 1), // 18
    /* 39 */ make_point(3, 1), // 19
};

// Ouput indices, responsible for x40 => x16 grouping
uint8_t groups[16] = { 0, 1, 4, 5, 10, 11, 14, 15, 20, 21, 24, 25, 30, 31, 34, 35, };

void vestMotorTransformer(std::string& value) {
    uint8_t result[40];

    // Unpack values
    for (auto i = 0; i < 20; i++) {
        uint8_t byte = value[i];
        uint actIndex = i * 2;

        result[actIndex] = (byte >> 4) & 0xf;
        result[actIndex + 1] = (byte & 0xf);
    }

    // Assign max value into each group
    for (auto i = 0; i < 16; i++) {
        auto groupIndex = groups[i];

        if (groupIndex % 10 >= 4) {
            auto maxValue = max(result[groupIndex], max(result[groupIndex+2], result[groupIndex+4]));

            result[groupIndex] = maxValue;
            result[groupIndex + 2] = maxValue;
            result[groupIndex + 4] = maxValue;
        } else {
            auto maxValue = max(result[groupIndex], result[groupIndex+2]);

            result[groupIndex] = maxValue;
            result[groupIndex + 2] = maxValue;
        }
    }

    for (uint8_t i = 0; i < 40; i++) {
        // take only meaningful values
        if (!contains(groups, i)) {
            continue;
        }

        outputData_t output;
        output.point = *indexesToPoints[i];
        output.intensity = map(result[i], 0, 15, 0, UINT16_MAX);
        App.getOutput()->writeOutput(
            (i < 10 || i >= 30) ? OUTPUT_PATH_CHEST_FRONT : OUTPUT_PATH_CHEST_BACK,
            output
        );
    }
}

#pragma endregion bHaptics_trash

void setupMode() {
    // Configure the PCA9685
    auto pwm = new Adafruit_PWMServoDriver(0x40);
    pwm->begin();
    pwm->setPWMFreq(PWM_FREQUENCY);

    // Assign the pins on the configured PCA9685 to positions on the vest
    auto frontOutputs = transformAutoOutput({
        { new PCA9685OutputWriter(pwm, 0),  new PCA9685OutputWriter(pwm, 1),  new PCA9685OutputWriter(pwm, 2),  new PCA9685OutputWriter(pwm, 3)  },
        { new PCA9685OutputWriter(pwm, 4),  new PCA9685OutputWriter(pwm, 5),  new PCA9685OutputWriter(pwm, 6),  new PCA9685OutputWriter(pwm, 7)  },
    });
    auto backOutputs = transformAutoOutput({
        { new PCA9685OutputWriter(pwm, 8),  new PCA9685OutputWriter(pwm, 9),  new PCA9685OutputWriter(pwm, 10), new PCA9685OutputWriter(pwm, 11) },
        { new PCA9685OutputWriter(pwm, 12), new PCA9685OutputWriter(pwm, 13), new PCA9685OutputWriter(pwm, 14), new PCA9685OutputWriter(pwm, 15) },
    });

    auto chestFront = new ClosestOutputComponent(frontOutputs);
    auto chestBack = new ClosestOutputComponent(backOutputs);

    App.getOutput()->addComponent(OUTPUT_PATH_CHEST_FRONT, chestFront);
    App.getOutput()->addComponent(OUTPUT_PATH_CHEST_BACK, chestBack);

    BHapticsBLEConnection* bhBleConnection = new BHapticsBLEConnection(BLUETOOTH_NAME, vestMotorTransformer);
    App.setConnection(bhBleConnection);

    #if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
        AbstractBattery* battery = new ADCBattery(33);
        App.setBattery(battery);
    #endif
}
