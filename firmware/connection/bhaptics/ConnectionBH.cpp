#include "connection/bhaptics/ConnectionBH.h"
#include "config.h"

MotorService* setupBhapticsMotorService(BLEServer* server)
{
    MotorService* service = new MotorService(server, BLE_SERVICE_MOTOR_UUID);
    return service;
}

ConnectionBH::ConnectionBH(std::string deviceName)
{
    this->deviceName = deviceName;

    this->bleServer = BLEDevice::createServer();

    this->motorService = setupBhapticsMotorService(this->bleServer);
}

ConnectionBH::~ConnectionBH()
{
    BLEDevice::deinit();
}

void ConnectionBH::init()
{
    BLEDevice::init(deviceName);

    this->motorService->init();
}

void ConnectionBH::start()
{
    this->motorService->start();
    
    this->bleServer->getAdvertising()->start();

    this->started = true;
}

void ConnectionBH::loop()
{
    
}

bool ConnectionBH::isOpen()
{
    return this->started;
}