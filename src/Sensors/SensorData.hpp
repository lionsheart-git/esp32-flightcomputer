/**
 * @file SensorData.hpp
 * @brief Functions for accessing data from all connected sensors.
 */

#ifndef FLIGHTCOMPUTER_SENSORDATA_HPP
#define FLIGHTCOMPUTER_SENSORDATA_HPP

#include <stdint.h>
#include <list>

#include <RTClib.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <Wire.h>

#include "BMP388.hpp"
#include "MPU6050.hpp"

#include "PinConfiguration.hpp"

class SensorData
{
public:
    SensorData();
    void Begin();

    void SmartDelay(unsigned long ms);
    void UpdateData();
    bool Calibrate();

    uint32_t GNSS_Satellites();
    int32_t GNSS_HDOP();
    double GNSS_Latitude();
    double GNSS_Longitude();
    double GNSS_Altitude();

    // Environment
    double Pressure();
    double AltitudeAboveGround();
    double AltitudeAboveSeaLevel();
    float Temperature();
    sensors_vec_t Acceleration() const;
    sensors_vec_t Gyro() const;

private:
    BMP388 bmp_;
    TinyGPSPlus gps_;
    HardwareSerial gpsSerial_;
    // TwoWire i2c_;
    MPU6050 mpu6050_;
};

extern SensorData Sensors;


#endif //FLIGHTCOMPUTER_SENSORDATA_HPP
