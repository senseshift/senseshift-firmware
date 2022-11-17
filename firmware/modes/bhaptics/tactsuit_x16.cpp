#include <Arduino.h>
#include <Wire.h>

#include "firmware.h"
#include "main.h"
#include "output.h"

#include "connections/bhaptics.h"
#include "outputs/auto_margins.h"
#include "output_writers/ledc.h"

// Front ouputs, responsible for x40 => x16 mappings

#define BH_X16_O_F00 0
#define BH_X16_O_F10 1
#define BH_X16_O_F20 30
#define BH_X16_O_F30 31
#define BH_X16_O_F01 8
#define BH_X16_O_F11 9
#define BH_X16_O_F21 38
#define BH_X16_O_F31 39

#define BH_X16_O_B00 10
#define BH_X16_O_B10 11
#define BH_X16_O_B20 20
#define BH_X16_O_B30 21
#define BH_X16_O_B01 18
#define BH_X16_O_B11 19
#define BH_X16_O_B21 28
#define BH_X16_O_B31 29

const uint16_t _bh_max_x = 4;
const uint16_t _bh_max_y = 2;

inline Point2D* make_point(uint16_t x, uint16_t y) {
    return new Point2D(UINT16_MAX * (1 / ((float)_bh_max_x - 1)) * ((float)x), UINT16_MAX * (1 / ((float)_bh_max_y - 1)) * ((float)y));
}

Point2D* indexesToPoints[40] = {
    // Front
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

    // Front
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

    // TODO: Debug, remove
    Serial.print("Before: { ");
    for (auto i = 0; i < 40; i++) {
        Serial.printf("%u, ", result[i]);
    }
    Serial.println(" }");

    // Assign max value into each group
    for (auto i = 0; i < 16; i++) {
        auto groupIndex = groups[i];

        if (groupIndex % 10 >= 4) {
            auto maxValue = max(result[groupIndex], max(result[groupIndex+2], result[groupIndex+4]));

            result[groupIndex] = maxValue;
            result[groupIndex+2] = maxValue;
            result[groupIndex+4] = maxValue;
        } else {
            auto maxValue = max(result[groupIndex], result[groupIndex+2]);

            result[groupIndex] = maxValue;
            result[groupIndex+2] = maxValue;
        }
    }

    // TODO: Debug, remove
    Serial.print("After: { ");
    for (auto i = 0; i < 40; i++) {
        Serial.printf("%u, ", result[i]);
    }
    Serial.println(" }");

    // Test 1
    // for (auto i = 0; i < 40; i++) {
    //     outputData_t output_0;
    //     output_0.point = *indexesToPoints[i];
    //     output_0.intensity = map(result[i], 0, 15, 0, UINT16_MAX);
    //     App.getOutput()->writeOutput(
    //         (i < 10 || i >= 30) ? OUTPUT_PATH_CHEST_FRONT : OUTPUT_PATH_CHEST_BACK,
    //         output_0
    //     );
    // }

    // Test 2
    for (auto i = 0; i < 40; i++) {
        // take only meaningful values
        if (i != BH_X16_O_F00 || i != BH_X16_O_F01 || i != BH_X16_O_F10 || i != BH_X16_O_F11 || i != BH_X16_O_F20 || i != BH_X16_O_F21 || i != BH_X16_O_F30 || i != BH_X16_O_F31
            || i != BH_X16_O_B00 || i != BH_X16_O_B01 || i != BH_X16_O_B10 || i != BH_X16_O_B11 || i != BH_X16_O_B20 || i != BH_X16_O_B21 || i != BH_X16_O_B30 || i != BH_X16_O_B31
        ) {
            continue;
        }

        outputData_t output_0;
        output_0.point = *indexesToPoints[i];
        output_0.intensity = map(result[i], 0, 15, 0, UINT16_MAX);
        App.getOutput()->writeOutput(
            (i < 10 || i >= 30) ? OUTPUT_PATH_CHEST_FRONT : OUTPUT_PATH_CHEST_BACK,
            output_0
        );
    }
}

void setupMode() {
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

    ledcSetup(8, 60, 12);
    ledcAttachPin(19, 8);

    ledcSetup(9, 60, 12);
    ledcAttachPin(18, 9);

    ledcSetup(10, 60, 12);
    ledcAttachPin(5, 10);

    ledcSetup(11, 60, 12);
    ledcAttachPin(17, 11);

    ledcSetup(12, 60, 12);
    ledcAttachPin(16, 12);

    ledcSetup(13, 60, 12);
    ledcAttachPin(4, 13);

    ledcSetup(14, 60, 12);
    ledcAttachPin(2, 14);

    ledcSetup(15, 60, 12);
    ledcAttachPin(15, 15);

    autoOutputVector_t frontOutputs{
        {
            new LEDCOutputWriter(0),
            new LEDCOutputWriter(1),
            new LEDCOutputWriter(2),
            new LEDCOutputWriter(3),
        },
        {
            new LEDCOutputWriter(4),
            new LEDCOutputWriter(5),
            new LEDCOutputWriter(6),
            new LEDCOutputWriter(7),
        },
    };
    autoOutputVector_t backOutputs{
        {
            new LEDCOutputWriter(8),
            new LEDCOutputWriter(9),
            new LEDCOutputWriter(10),
            new LEDCOutputWriter(11),
        },
        {
            new LEDCOutputWriter(12),
            new LEDCOutputWriter(13),
            new LEDCOutputWriter(14),
            new LEDCOutputWriter(15),
        },
    };

    OutputComponent* chestFront = new OutputAutoComponent_Margin(frontOutputs);
    OutputComponent* chestBack = new OutputAutoComponent_Margin(backOutputs);

    App.getOutput()->addComponent(OUTPUT_PATH_CHEST_FRONT, chestFront);
    App.getOutput()->addComponent(OUTPUT_PATH_CHEST_BACK, chestBack);

    BHapticsBLEConnection* bhBleConnection = new BHapticsBLEConnection(BLUETOOTH_NAME, vestMotorTransformer);
    App.registerComponent(bhBleConnection);
}