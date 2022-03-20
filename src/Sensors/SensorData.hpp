#ifndef FLIGHTCOMPUTER_SENSORDATA_HPP
#define FLIGHTCOMPUTER_SENSORDATA_HPP

#include <stdint.h>
#include <list>

#include <RTClib.h>
#include <TinyGPS++.h>
#include <Adafruit_BMP3XX.h>
#include <HardwareSerial.h>
#include <Wire.h>

#include "PinConfiguration.hpp"

class SensorData
{
public:
    SensorData();
    void Begin();

    void SmartDelay(unsigned long ms);

    //GNSS
    uint32_t Satellites();
    int32_t HDOP();
    double Latitude();
    double Longitude();

    // Date and time
    DateTime Time();
    uint8_t Hour();
    uint8_t Minute();
    uint8_t Second();
    uint32_t Date();
    uint16_t Year();
    uint8_t Month();
    uint8_t Day();
    const char * Timestamp();

    // Environment
    double Pressure();
    double Altitude();
    float Temperature();
    float Humidity();
    float UV();
    float Lux();

private:
    RTC_DS3231 rtc_;
    Adafruit_BMP3XX bmp_;
    TinyGPSPlus gps_;
    HardwareSerial gpsSerial_;
    // TwoWire i2c_;
};

extern SensorData Sensors;


#endif //FLIGHTCOMPUTER_SENSORDATA_HPP
