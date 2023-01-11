// WARNING: this code is not intended to use as of now
#ifndef CI
#error \
    "Change your build target. (See https://openhaptics.github.io/docs/flashing/platformio#building-and-uploading-firmware)"
#endif

// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include <utility.hpp>

#include "openhaptics.h"
#include "auto_output.h"

#include "connections/bhaptics.h"
#include "output_components/closest.h"
#include "output_writers/pca9685.h"

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include "battery/adc_battery.h"
#endif

#if defined(OHA_CHEST_FRONT_ROWS) && defined(OHA_CHEST_FRONT_COLS)
#define OHA_CHEST_FRONT_NUM (OHA_CHEST_FRONT_ROWS * OHA_CHEST_FRONT_COLS)
#endif

#if defined(OHA_CHEST_BACK_ROWS) && defined(OHA_CHEST_BACK_COLS)
#define OHA_CHEST_BACK_NUM (OHA_CHEST_BACK_ROWS * OHA_CHEST_BACK_COLS)
#endif

#ifndef OHA_CHEST_FRONT_NUM
#define OHA_CHEST_FRONT_NUM 0
#endif

#ifndef OHA_CHEST_BACK_NUM
#define OHA_CHEST_BACK_NUM 0
#endif

#define OHA_TOTAL_NUM (OHA_CHEST_FRONT_NUM + OHA_CHEST_BACK_NUM)

using namespace OH;

std::vector<std::vector<AbstractOutputWriter*>> createAutoOutputs(
    uint rows_num,
    uint cols_num,
    std::vector<Adafruit_PWMServoDriver*>& pwms,
    uint offset) {
  std::vector<std::vector<AbstractOutputWriter*>> rows{};

  for (uint i = 0; i < rows_num; i++) {
    std::vector<AbstractOutputWriter*> row{};
    for (uint j = 0; j < cols_num; j++) {
      const uint index = offset + (i * j);
      row.push_back(new PCA9685OutputWriter(pwms.at(index % 16), index / 16));
    }
    rows.push_back(row);
  }

  return rows;
}

void setupMode() {
  std::vector<Adafruit_PWMServoDriver*> pwms(OHA_TOTAL_NUM);
  for (uint i = 0; i < OHA_TOTAL_NUM; i++) {
    Adafruit_PWMServoDriver* pwm = new Adafruit_PWMServoDriver(0x40 + i);
    pwm->begin();
    pwm->setPWMFreq(60);

    pwms[i] = pwm;
  }

  uint offset = 0;

#if OHA_CHEST_FRONT_NUM != 0
  auto frontOutputs = createAutoOutputs(OHA_CHEST_FRONT_ROWS,
                                        OHA_CHEST_FRONT_COLS, pwms, offset);
  auto front = transformAutoOutput(frontOutputs);
  App.getOutput()->addComponent(OUTPUT_PATH_CHEST_FRONT,
                                new ClosestOutputComponent(front));
  offset += OHA_CHEST_FRONT_NUM;
#endif

#if OHA_CHEST_BACK_NUM != 0
  auto backOutputs =
      createAutoOutputs(OHA_CHEST_BACK_ROWS, OHA_CHEST_BACK_COLS, pwms, offset);
  auto back = transformAutoOutput(backOutputs);
  App.getOutput()->addComponent(OUTPUT_PATH_CHEST_BACK,
                                new ClosestOutputComponent(back));
  offset += OHA_CHEST_BACK_NUM;
#endif
}
