#ifndef BH_SERVICE_H
#define BH_SERVICE_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

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

#endif /* BH_SERVICE_H */