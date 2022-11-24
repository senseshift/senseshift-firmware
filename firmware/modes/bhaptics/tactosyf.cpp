#include <Arduino.h>
#include <Wire.h>

#include "openhaptics.h"
#include "utils.h"
#include "auto_output.h"

#include "connections/bhaptics.h"
#include "output_components/closest.h"
#include "output_writers/ledc.h"

#pragma region bHaptics_trash

const uint16_t _bh_max_x = 3;
const uint16_t _bh_max_y = 1;

inline Point2D* make_point(uint16_t x, uint16_t y) {
    return getPoint(x, y, _bh_max_x, _bh_max_y);
}

Point2D* indexesToPoints[_bh_max_x * _bh_max_y] = {
    make_point(0, 0),
    make_point(1, 0),
    make_point(2, 0)
};

void vestMotorTransformer(std::string& value) {
    for (size_t i = 0; i < _bh_max_x; i++) {
        uint8_t byte = value[i];
        outputData_t output_0;
        output_0.point = *indexesToPoints[i];
        output_0.intensity = map(byte, 0, 100, 0, UINT16_MAX);
        App.getOutput()->writeOutput(OUTPUT_PATH_ACCESSORY, output_0);
    }
}

#pragma endregion bHaptics_trash

void setupMode() {
    // Configure PWM pins to their positions on the feet
    auto footOutputs = transformAutoOutput({
        { new LEDCOutputWriter(32), new LEDCOutputWriter(33), new LEDCOutputWriter(25) }
    });

    auto foot = new ClosestOutputComponent(footOutputs);

    App.getOutput()->addComponent(OUTPUT_PATH_ACCESSORY, foot);

    BHapticsBLEConnection* bhBleConnection = new BHapticsBLEConnection(BLUETOOTH_NAME, vestMotorTransformer);
    App.setConnection(bhBleConnection);
}