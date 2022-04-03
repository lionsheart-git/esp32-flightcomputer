
#ifndef ESP32_FLIGHTCOMPUTER_MPU6050_HPP
#define ESP32_FLIGHTCOMPUTER_MPU6050_HPP

#include <Adafruit_MPU6050.h>

class MPU6050 : public Adafruit_MPU6050
{
public:
    bool Calibrate();
    void Update();

    sensors_vec_t acceleration_, gyro_;
    float temperature_;
};


#endif //ESP32_FLIGHTCOMPUTER_MPU6050_HPP
