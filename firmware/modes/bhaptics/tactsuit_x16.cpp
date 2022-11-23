#include <Arduino.h>
#include <Wire.h>

#include "openhaptics.h"
#include "utils.h"
#include "auto_output.h"

#include "connections/bhaptics.h"
#include "output_components/closest.h"
#include "output_writers/ledc.h"

#define PWM_FREQUENCY 60
#define PWM_RESOLUTION 12

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

    ledcSetup(8, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(19, 8);

    ledcSetup(9, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(18, 9);

    ledcSetup(10, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(5, 10);

    ledcSetup(11, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(17, 11);

    ledcSetup(12, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(16, 12);

    ledcSetup(13, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(4, 13);

    ledcSetup(14, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(2, 14);

    ledcSetup(15, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(15, 15);

    // Map the above channels to their positions on the vest
    auto frontOutputs = transformAutoOutput({
        { new LEDCOutputWriter(0),  new LEDCOutputWriter(1),  new LEDCOutputWriter(2),  new LEDCOutputWriter(3) },
        { new LEDCOutputWriter(4),  new LEDCOutputWriter(5),  new LEDCOutputWriter(6),  new LEDCOutputWriter(7) },
    });
    auto backOutputs = transformAutoOutput({
        { new LEDCOutputWriter(8),  new LEDCOutputWriter(9),  new LEDCOutputWriter(10), new LEDCOutputWriter(11) },
        { new LEDCOutputWriter(12), new LEDCOutputWriter(13), new LEDCOutputWriter(14), new LEDCOutputWriter(15) },
    });

    auto chestFront = new ClosestOutputComponent(frontOutputs);
    auto chestBack = new ClosestOutputComponent(backOutputs);

    App.getOutput()->addComponent(OUTPUT_PATH_CHEST_FRONT, chestFront);
    App.getOutput()->addComponent(OUTPUT_PATH_CHEST_BACK, chestBack);

    BHapticsBLEConnection* bhBleConnection = new BHapticsBLEConnection(BLUETOOTH_NAME, vestMotorTransformer);
    App.setConnection(bhBleConnection);
}