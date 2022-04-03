
#include "MPU6050.hpp"

bool MPU6050::Calibrate()
{
    return true;
}

void MPU6050::Update()
{
    sensors_event_t acceleration, gyro, temperature;

    this->getEvent(&acceleration, &gyro, &temperature);
    this->acceleration_ = acceleration.acceleration;
    this->gyro_ = gyro.gyro;
    this->temperature_ = temperature.temperature;
}
