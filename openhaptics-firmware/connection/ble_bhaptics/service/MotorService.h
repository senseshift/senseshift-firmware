#ifndef MOTOR_SERVICE_H
#define MOTOR_SERVICE_H

#include "BHService.h"

#include "../../../output/Output.h"

class MotorService : public BHService
{
    private:
        BLECharacteristic* charMotor;
        BLECharacteristic* charMotorStable;

    public:
        MotorService(BLEServer*, BLEUUID); // todo: add output

        void init(void);
};

#endif /* MOTOR_SERVICE_H */
