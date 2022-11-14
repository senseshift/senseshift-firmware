#include <Arduino.h>
#include <Wire.h>

#include "firmware.h"
#include "main.h"
#include "output.h"

#include "connections/bhaptics.h"
#include "outputs/auto_margins.h"
#include "output_writers/ledc.h"

const uint16_t _bh_max_x = 4;
const uint16_t _bh_max_y = 2;

inline Point2D* make_point(uint16_t x, uint16_t y) {
    return new Point2D(UINT16_MAX * (1 / ((float)_bh_max_x - 1)) * ((float)x), UINT16_MAX * (1 / ((float)_bh_max_y - 1)) * ((float)y));
}

Point2D* indexesToPoints[40] = {
    // Front
    make_point(0, 0), // 0
    make_point(1, 0), // 1
    make_point(0, 0), // 4
    make_point(1, 0), // 5

    make_point(0, 1), // 8
    make_point(1, 1), // 9
    make_point(0, 1), // 12
    make_point(1, 1), // 13
    make_point(0, 1), // 16
    make_point(1, 1), // 17

    // Back
    make_point(0, 0), // 0
    make_point(1, 0), // 1
    make_point(0, 0), // 4
    make_point(1, 0), // 5

    make_point(0, 1), // 8
    make_point(1, 1), // 9
    make_point(0, 1), // 12
    make_point(1, 1), // 13
    make_point(0, 1), // 16
    make_point(1, 1), // 17

    make_point(2, 0), // 2
    make_point(3, 0), // 3
    make_point(2, 0), // 4
    make_point(3, 0), // 7

    make_point(2, 1), // 10
    make_point(3, 1), // 11
    make_point(2, 1), // 14
    make_point(3, 1), // 15
    make_point(2, 1), // 18
    make_point(3, 1), // 19

    // Front
    make_point(2, 0), // 2
    make_point(3, 0), // 3
    make_point(2, 0), // 4
    make_point(3, 0), // 7

    make_point(2, 1), // 10
    make_point(3, 1), // 11
    make_point(2, 1), // 14
    make_point(3, 1), // 15
    make_point(2, 1), // 18
    make_point(3, 1), // 19
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
        },
        {
            new LEDCOutputWriter(2),
            new LEDCOutputWriter(3),
        },
        {
            new LEDCOutputWriter(4),
            new LEDCOutputWriter(5),
        },
        {
            new LEDCOutputWriter(6),
            new LEDCOutputWriter(7),
        },
    };
    autoOutputVector_t backOutputs{
        {
            new LEDCOutputWriter(8),
            new LEDCOutputWriter(9),
        },
        {
            new LEDCOutputWriter(10),
            new LEDCOutputWriter(11),
        },
        {
            new LEDCOutputWriter(12),
            new LEDCOutputWriter(13),
        },
        {
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