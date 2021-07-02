#include "firmware.h"
#include "main.h"

#include "outputs/auto_margins.h"
#include "output_writers/pca9685.h"

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

autoOutputVector_t createAutoOutputs(uint rows_num, uint cols_num, std::vector<Adafruit_PWMServoDriver*> &pwms, uint offset)
{
    autoOutputVector_t rows {};

    for (uint i = 0; i < rows_num; i++)
    {
        std::vector<OutputWriter*> row {};
        for (uint j = 0; j < cols_num; j++)
        {
            const uint index = offset + (i*j);
            row.push_back(new PCA9685OutputWriter(pwms.at(index % 16), index / 16));
        }
        rows.push_back(row);
    }

    return rows;
}

void setupMode()
{
    std::vector<Adafruit_PWMServoDriver*> pwms(OHA_TOTAL_NUM);
    for (uint i = 0; i < OHA_TOTAL_NUM; i++)
    {
        Adafruit_PWMServoDriver* pwm = new Adafruit_PWMServoDriver(0x40 + i);
        pwm->begin();
        pwm->setPWMFreq(60);

        pwms[i] = pwm;
    }

    uint offset = 0;

    #if OHA_CHEST_FRONT_NUM != 0
        auto frontOutputs = createAutoOutputs(OHA_CHEST_FRONT_ROWS, OHA_CHEST_FRONT_COLS, pwms, offset);
        App.getOutput()->addComponent(OUTPUT_PATH_CHEST_FRONT, new OutputAutoComponent_Margin(frontOutputs));
        offset += OHA_CHEST_FRONT_NUM;
    #endif

    #if OHA_CHEST_BACK_NUM != 0
        auto backOutputs = createAutoOutputs(OHA_CHEST_BACK_ROWS, OHA_CHEST_BACK_COLS, pwms, offset);
        App.getOutput()->addComponent(OUTPUT_PATH_CHEST_BACK, new OutputAutoComponent_Margin(backOutputs));
        offset += OHA_CHEST_BACK_NUM;
    #endif
}
