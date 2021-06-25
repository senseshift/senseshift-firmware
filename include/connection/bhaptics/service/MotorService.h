#pragma once

#include "./BHService.h"

#include "output/Output.h"

class MotorService : public BHService
{
    private:
        BLECharacteristic* charMotor;
        BLECharacteristic* charMotorStable;

    public:
        MotorService(BLEServer*, BLEUUID); // todo: add output

        void init(void);
};

