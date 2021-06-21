#include "BHService.h"

BHService::BHService(BLEServer* server, BLEUUID uuid)
{
    this->bleService = server->createService(uuid);
    server->getAdvertising()->addServiceUUID(uuid);
}

BHService::~BHService()
{
    this->bleService->getServer()->removeService(this->bleService);
}

void BHService::start()
{
    this->getBleService()->start();
}