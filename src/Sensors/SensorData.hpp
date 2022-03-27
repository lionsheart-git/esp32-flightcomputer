#ifndef FLIGHTCOMPUTER_SENSORDATA_HPP
#define FLIGHTCOMPUTER_SENSORDATA_HPP

#include <stdint.h>
#include <list>

#include <RTClib.h>
#include <TinyGPS++.h>
#include "BMP388.hpp"
#include <HardwareSerial.h>
#include <Wire.h>

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

    // Environment
    double Pressure();
    double Altitude();
    float Temperature();
    float Humidity();
    float UV();
    float Lux();

private:
    BMP388 bmp_;
    TinyGPSPlus gps_;
    HardwareSerial gpsSerial_;
    // TwoWire i2c_;
};

extern SensorData Sensors;


#endif //FLIGHTCOMPUTER_SENSORDATA_HPP
