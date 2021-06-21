#include "ConnectionBLE.h"

#include "service/MotorService.cpp"

ConnectionBLE::ConnectionBLE(std::string deviceName)
{
    this->deviceName = deviceName;
    
    this->bleServer = BLEDevice::createServer();

    this->motorService = this->setupMotorService(this->bleServer);
}

ConnectionBLE::~ConnectionBLE()
{
    BLEDevice::deinit();
}

MotorService* ConnectionBLE::setupMotorService(BLEServer* server)
{
    MotorService* service = new MotorService(server, BLE_SERVICE_MOTOR_UUID);
    return service;
}

void ConnectionBLE::init()
{
    BLEDevice::init(deviceName);

    this->motorService->init();
}

void ConnectionBLE::start()
{
    this->motorService->start();
    
    this->bleServer->getAdvertising()->start();

    this->started = true;
}

void ConnectionBLE::loop()
{
    
}

bool ConnectionBLE::isOpen()
{
    return this->started;
}