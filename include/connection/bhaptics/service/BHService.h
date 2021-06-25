#pragma once

#include <BLEServer.h>
#include <BLEUtils.h>

class BHService
{
    protected:
        BLEService* bleService;

    public:
        BHService(BLEServer*, BLEUUID);
        ~BHService(void);
        BLEService* getBleService(void) { return this->bleService; };

        virtual void init(void) = 0;
        void start(void);
};
