#include "SensorData.hpp"

#include <Arduino.h>

#include "Logging/SystemLogger.hpp"

void SensorData::SmartDelay(unsigned long ms)
{
    unsigned long start = millis();
    do
    {
        // Call ReadSerial for all GPS objects
    } while (millis() - start < ms);
}

//@todo Figure out whether to use exceptions or return codes to signal invalid readings
uint32_t SensorData::Satellites()
{
    uint32_t satellites = 0;

    return satellites;
}

int32_t SensorData::HDOP()
{
    int32_t hdop = 0;

    return hdop;
}

double SensorData::Latitude()
{
    double latitude = 0;

    return latitude;
}

double SensorData::Longitude()
{
    double longitude = 0;

    return longitude;
}

Time_t SensorData::Time()
{
    return Time_t();
}

uint8_t SensorData::Hour()
{
    return 0;
}

uint8_t SensorData::Minute()
{
    return 0;
}

uint8_t SensorData::Second()
{
    return 0;
}

uint32_t SensorData::Date()
{
    return 0;
}

uint16_t SensorData::Year()
{
    return 0;
}

uint8_t SensorData::Month()
{
    return 0;
}

uint8_t SensorData::Day()
{
    return 0;
}

double SensorData::Altitude()
{
    return 0;
}

float SensorData::Temperature()
{
    return 0;
}

float SensorData::Humidity()
{
    return 0;
}

float SensorData::UV()
{
    return 0;
}

float SensorData::Lux()
{
    return 0;
}
