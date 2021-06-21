#include "MotorService.h"

#include <BLE2902.h> 

#include "BHService.cpp"

MotorService::MotorService(BLEServer* server, BLEUUID uuid) : BHService(server, uuid) 
{
    this->charMotor = new BLECharacteristic(
        BLE_SERVICE_MOTOR_CHAR_MOTOR_UUID, 
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
    );

    this->charMotor->addDescriptor(new BLE2902());
}

void MotorService::init()
{

}